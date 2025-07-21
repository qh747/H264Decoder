#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Utils/NaluHelper.h"
#include "Stream/NaluStream.h"
#include "AnnexB/NaluSliceHeader.h"
using Common::Nalu_t;
using Common::Slice_t;
using Utils::NaluHelper;
using Stream::NaluStream;

namespace AnnexB {

bool NaluSliceHeader::parse(const std::vector<uint8_t>& buffer, const NaluSPS& sps, const NaluPPS& pps) {
    if (buffer.empty() || !sps.m_isValid || !pps.m_isValid) {
        std::cerr << "Parse nalu slice header error. input param invalid." << std::endl;
        return false;
    }

    m_headParam = Common::SliceHeadParam_dt();
    NaluStream ns(buffer.data(), buffer.size());

    // 读取nalu类型
    int head = ns.readNBits(8);
    auto type = static_cast<Nalu_t>(head & 0x1f);
    if (Nalu_t::IDRSlice == type) {
        m_isIdrSlice = true;
    }

    auto nal_ref_idc = static_cast<int>((head >> 5) & 3);

    m_headParam.first_mb_in_slice = ns.readUev();
    m_headParam.slice_type = ns.readUev();
    m_headParam.pic_parameter_set_id = ns.readUev();

    if (1 == sps.m_spsParam.separate_colour_plane_flag) {
        m_headParam.colour_plane_id = ns.readNBits(2);
    }

    // 7.4.3 Slice header semantics
    m_headParam.frame_num = ns.readNBits(sps.m_spsParam.log2_max_frame_num_minus4 + 4);

    if (0 == sps.m_spsParam.frame_mbs_only_flag) {
        m_headParam.field_pic_flag = ns.readOneBit();
        if (1 == m_headParam.field_pic_flag) {
            m_headParam.bottom_field_flag = ns.readOneBit();
        }
    }

    if (Nalu_t::IDRSlice == type) {
        m_headParam.idr_pic_id = ns.readUev();
    }

    if (0 == sps.m_spsParam.pic_order_cnt_type) {
        // 7.4.3 Slice header semantics
        m_headParam.pic_order_cnt_lsb = ns.readNBits(sps.m_spsParam.log2_max_frame_num_minus4 + 4);
        if (0 != pps.m_ppsParam.bottom_field_pic_order_in_frame_present_flag && 0 == m_headParam.field_pic_flag) {
            m_headParam.delta_pic_order_cnt_bottom = ns.readSev();
        }
    }

    if (1 == sps.m_spsParam.pic_order_cnt_type && 0 == sps.m_spsParam.delta_pic_order_always_zero_flag) {
        m_headParam.delta_pic_order_cnt[0] = ns.readSev();
        if (0 != pps.m_ppsParam.bottom_field_pic_order_in_frame_present_flag && 0 == m_headParam.field_pic_flag) {
            m_headParam.delta_pic_order_cnt[1] = ns.readSev();
        }
    }

    if (0 != pps.m_ppsParam.redundant_pic_cnt_present_flag) {
        m_headParam.redundant_pic_cnt = ns.readUev();
    }

    Slice_t sliceType;
    if (!NaluHelper::GetSliceType(m_headParam.slice_type, type, sps.m_spsParam.max_num_ref_frames, sliceType)) {
        std::cerr << "Parse nalu slice header error. slice type error. type: " << m_headParam.slice_type
            << " nalu type: " << static_cast<int>(type) << " max num ref frames: " << sps.m_spsParam.max_num_ref_frames << std::endl;
        return false;
    }

    if (Slice_t::B_SLICE == sliceType) {
        m_headParam.direct_spatial_mv_pred_flag = ns.readOneBit();
    }

    if (Slice_t::P_SLICE == sliceType || Slice_t::SP_SLICE == sliceType || Slice_t::B_SLICE == sliceType) {
        m_headParam.num_ref_idx_active_override_flag = ns.readOneBit();
        if (1 == m_headParam.num_ref_idx_active_override_flag) {
            m_headParam.num_ref_idx_l0_active_minus1 = ns.readUev();
            if (Slice_t::B_SLICE == sliceType) {
                m_headParam.num_ref_idx_l1_active_minus1 = ns.readUev();
            }
        }
    }

    if (Nalu_t::CodedSliceExt == type || Nalu_t::CodedSliceExt2 == type) {
        std::cerr << "Parse nalu slice header error. method ref_pic_list_mvc_modification() not support. nalu type: "
            << static_cast<int>(type) << std::endl;
        return false;
    }
    else {
        NaluHelper::ParseRefPicListModification(ns, sliceType, m_headParam);
    }

    if (0 != pps.m_ppsParam.weighted_pred_flag && (Slice_t::P_SLICE == sliceType || Slice_t::SP_SLICE == sliceType) ||
        (1 == pps.m_ppsParam.weighted_bipred_idc && Slice_t::B_SLICE == sliceType)) {
        uint32_t chromaArrayType = 0 == sps.m_spsParam.separate_colour_plane_flag ?
            sps.m_spsParam.chroma_format_idc : 0;

        NaluHelper::ParsePredWeightTable(ns, sliceType, chromaArrayType, m_headParam);
    }

    if (0 != nal_ref_idc) {
        NaluHelper::ParseDecRefPisMark(ns, type, m_headParam);
    }

    if (0 != pps.m_ppsParam.entropy_coding_mode_flag && Slice_t::I_SLICE != sliceType && Slice_t::SI_SLICE != sliceType) {
        m_headParam.cabac_init_idc = ns.readUev();
    }

    m_headParam.slice_qp_delta = ns.readSev();

    if (Slice_t::SP_SLICE == sliceType || Slice_t::SI_SLICE == sliceType) {
        if (Slice_t::SP_SLICE == sliceType) {
            m_headParam.sp_for_switch_flag = ns.readOneBit();
        }
        m_headParam.slice_qs_delta = ns.readSev();
    }

    if (0 != pps.m_ppsParam.deblocking_filter_control_present_flag) {
        m_headParam.disable_deblocking_filter_idc = ns.readUev();
        if (1 != m_headParam.disable_deblocking_filter_idc) {
            m_headParam.slice_alpha_c0_offset_div2 = ns.readSev();
            m_headParam.slice_beta_offset_div2 = ns.readSev();
        }
    }

    if (pps.m_ppsParam.num_slice_groups_minus1 > 0 && pps.m_ppsParam.slice_group_map_type >= 3 && pps.m_ppsParam.slice_group_map_type <= 5) {
        /**
         * 7.4.2.1.1 Sequence parameter set data semantics
         * slice_group_change_cycle = Ceil(Log2(PicSizeInMapUnits / SliceGroupChangeRate + 1))
         *
         * PicSizeInMapUnits = PicWidthInMbs * PicHeightInMapUnits
         *     PicWidthInMbs = pic_width_in_mbs_minus1 + 1
         *     PicHeightInMapUnits = pic_height_in_map_units_minus1 + 1
         *
         * SliceGroupChangeRate = slice_group_change_rate_minus1 + 1
         *
         * Log2(): 求形参v是2的多少次幂
         * Ceil(): 向上取整
         */

        uint32_t picSizeInMapUnits = (sps.m_spsParam.pic_width_in_mbs_minus1 + 1) * (sps.m_spsParam.pic_height_in_map_units_minus1 + 1);
        uint32_t sliceGroupChangeRate = (pps.m_ppsParam.slice_group_change_rate_minus1 + 1);
        double bitsLen = ceil(log2(static_cast<double>(picSizeInMapUnits) / static_cast<double>(sliceGroupChangeRate) + 1));
        m_headParam.slice_group_change_cycle = ns.readNBits(static_cast<std::size_t>(bitsLen));
    }

    m_isValid = true;
    return true;
}

std::string NaluSliceHeader::dumpString() const {
    if (!m_isValid) {
        return "";
    }

    std::stringstream ss;
    ss << "SLICE HEADER ========================" << std::endl;
    ss << "first_mb_in_slice: " << m_headParam.first_mb_in_slice << std::endl;
    ss << "slice_type: " << m_headParam.slice_type << std::endl;
    ss << "pic_parameter_set_id: " << m_headParam.pic_parameter_set_id << std::endl;
    ss << "colour_plane_id: " << m_headParam.colour_plane_id << std::endl;
    ss << "frame_num: " << m_headParam.frame_num << std::endl;
    ss << "field_pic_flag: " << static_cast<int>(m_headParam.field_pic_flag) << std::endl;
    ss << "bottom_field_flag: " << static_cast<int>(m_headParam.bottom_field_flag) << std::endl;
    ss << "idr_pic_id: " << m_headParam.idr_pic_id << std::endl;
    ss << "pic_order_cnt_lsb: " << m_headParam.pic_order_cnt_lsb << std::endl;
    ss << "delta_pic_order_cnt_bottom: " << m_headParam.delta_pic_order_cnt_bottom << std::endl;

    ss << "delta_pic_order_cnt: ";
    for (int i = 0; i < 2; ++i) {
        ss << m_headParam.delta_pic_order_cnt[i] << " ";
    }
    ss << std::endl;

    ss << "redundant_pic_cnt: " << m_headParam.redundant_pic_cnt << std::endl;
    ss << "direct_spatial_mv_pred_flag: " << static_cast<int>(m_headParam.direct_spatial_mv_pred_flag) << std::endl;
    ss << "num_ref_idx_active_override_flag: " << static_cast<int>(m_headParam.num_ref_idx_active_override_flag) << std::endl;
    ss << "num_ref_idx_l0_active_minus1: " << m_headParam.num_ref_idx_l0_active_minus1 << std::endl;
    ss << "num_ref_idx_l1_active_minus1: " << m_headParam.num_ref_idx_l1_active_minus1 << std::endl;
    ss << "cabac_init_idc: " << m_headParam.cabac_init_idc << std::endl;
    ss << "slice_qp_delta: " << m_headParam.slice_qp_delta << std::endl;
    ss << "sp_for_switch_flag: " << static_cast<int>(m_headParam.sp_for_switch_flag) << std::endl;
    ss << "slice_qs_delta: " << m_headParam.slice_qs_delta << std::endl;
    ss << "disable_deblocking_filter_idc: " << m_headParam.disable_deblocking_filter_idc << std::endl;
    ss << "slice_alpha_c0_offset_div2: " << m_headParam.slice_alpha_c0_offset_div2 << std::endl;
    ss << "slice_beta_offset_div2: " << m_headParam.slice_beta_offset_div2 << std::endl;
    ss << "slice_group_change_cycle: " << m_headParam.slice_group_change_cycle << std::endl;

    ss << "REF PIC LIST MODIFICATION -------------------" << std::endl;
    const auto& refPicListModification = m_headParam.ref_pic_list_modification;
    ss << "modification_of_pic_nums_idc" << refPicListModification.modification_of_pic_nums_idc << std::endl;
    ss << "abs_diff_pic_num_minus1: " << refPicListModification.abs_diff_pic_num_minus1 << std::endl;
    ss << "long_term_pic_num: " << refPicListModification.long_term_pic_num << std::endl;
    ss << "ref_pic_list_modification_flag_l0: " << static_cast<int>(refPicListModification.ref_pic_list_modification_flag_l0) << std::endl;
    ss << "ref_pic_list_modification_flag_l1: " << static_cast<int>(refPicListModification.ref_pic_list_modification_flag_l1) << std::endl;

    ss << "PREDICTION WEIGHT TABLE -------------------" << std::endl;
    const auto& predWeightTable = m_headParam.pred_weight_table;
    ss << "luma_log2_weight_denom: " << predWeightTable.luma_log2_weight_denom << std::endl;
    ss << "chroma_log2_weight_denom: " << predWeightTable.chroma_log2_weight_denom << std::endl;

    ss << "luma_weight_l0_flag: " << static_cast<int>(predWeightTable.luma_weight_l0_flag) << std::endl;
    ss << "luma_weight_l0";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.luma_weight_l0[idx] << " ";
    }
    ss << std::endl;

    ss << "luma_offset_l0";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.luma_offset_l0[idx] << " ";
    }
    ss << std::endl;

    ss << "chroma_weight_l0_flag: " << static_cast<int>(predWeightTable.chroma_weight_l0_flag) << std::endl;
    ss << "chroma_weight_l0";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.chroma_weight_l0[idx][0] << " " << predWeightTable.chroma_weight_l0[idx][1] << " ";
    }
    ss << std::endl;

    ss << "chroma_offset_l0";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.chroma_offset_l0[idx][0] << " "<< predWeightTable.chroma_offset_l0[idx][1] << " ";
    }
    ss << std::endl;

    ss << "luma_weight_l1_flag: " << static_cast<int>(predWeightTable.luma_weight_l1_flag) << std::endl;
    ss << "luma_weight_l1";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.luma_weight_l1[idx] << " ";
    }
    ss << std::endl;

    ss << "luma_offset_l1";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.luma_offset_l1[idx] << " ";
    }
    ss << std::endl;

    ss << "chroma_weight_l1_flag: " << static_cast<int>(predWeightTable.chroma_weight_l1_flag) << std::endl;
    ss << "chroma_weight_l1";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.chroma_weight_l1[idx][0] << " " << predWeightTable.chroma_weight_l1[idx][1] << " ";
    }
    ss << std::endl;

    ss << "chroma_offset_l1";
    for (auto idx = 0; idx < 32; ++idx) {
        ss << predWeightTable.chroma_offset_l1[idx][0] << " " << predWeightTable.chroma_offset_l1[idx][1] << " ";
    }
    ss << std::endl;

    ss << "DEC REF PIS MARKING -------------------" << std::endl;
    const auto& decRefPicMarking = m_headParam.dec_ref_pic_marking;
    ss << "no_output_of_prior_pics_flag: " << decRefPicMarking.no_output_of_prior_pics_flag << std::endl;
    ss << "long_term_reference_flag: " << decRefPicMarking.long_term_reference_flag << std::endl;
    ss << "adaptive_ref_pic_marking_mode_flag: " << decRefPicMarking.adaptive_ref_pic_marking_mode_flag << std::endl;
    ss << "memory_management_control_operation: " << decRefPicMarking.memory_management_control_operation << std::endl;
    ss << "difference_of_pic_nums_minus1: " << decRefPicMarking.difference_of_pic_nums_minus1 << std::endl;
    ss << "long_term_pic_num: " << decRefPicMarking.long_term_pic_num << std::endl;
    ss << "long_term_frame_idx: " << decRefPicMarking.long_term_frame_idx << std::endl;
    ss << "max_long_term_frame_idx_plus1: " << decRefPicMarking.max_long_term_frame_idx_plus1 << std::endl;

    return ss.str();
}

}; // namespace AnnexB