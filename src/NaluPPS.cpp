#include <sstream>
#include <iostream>
#include "Utils/NaluHelper.h"
#include "Stream/NaluStream.h"
#include "AnnexB/NaluPPS.h"
using Common::Nalu_t;
using Utils::NaluHelper;
using Stream::NaluStream;

namespace AnnexB {

bool NaluPPS::parse(const std::vector<uint8_t>& buffer, const NaluSPS& sps)
{
    if (buffer.empty() || !sps.m_isValid) {
        std::cerr << "Parse nalu pps error. input param invalid." << std::endl;
        return false;
    }

    NaluStream ns(buffer.data(), buffer.size());

    // 读取nalu类型
    int head = ns.readNBits(8);
    auto type = static_cast<Nalu_t>(head & 0x1f);
    if (Nalu_t::PPS != type) {
        std::cerr << "Parse nalu pps error. nalu type invalid. type: " << static_cast<int>(type) << std::endl;
        return false;
    }

    m_ppsParam.pic_parameter_set_id = ns.readUev();
    m_ppsParam.seq_parameter_set_id = ns.readUev();

    m_ppsParam.entropy_coding_mode_flag = ns.readOneBit();
    m_ppsParam.bottom_field_pic_order_in_frame_present_flag = ns.readOneBit();

    m_ppsParam.num_slice_groups_minus1 = ns.readUev();
    if (m_ppsParam.num_slice_groups_minus1 > 0) {
        m_ppsParam.slice_group_map_type = ns.readUev();
        if (0 == m_ppsParam.slice_group_map_type) {
            for (auto iGroup = 0; iGroup <= m_ppsParam.num_slice_groups_minus1; ++iGroup) {
                m_ppsParam.run_length_minus1[iGroup] = ns.readUev();
            }
        }
        else if (2 == m_ppsParam.slice_group_map_type) {
            for (auto iGroup = 0; iGroup < m_ppsParam.num_slice_groups_minus1; ++iGroup) {
                m_ppsParam.top_left[iGroup] = ns.readUev();
                m_ppsParam.bottom_right[iGroup] = ns.readUev();
            }
        }
        else if (3 <= m_ppsParam.slice_group_map_type && m_ppsParam.slice_group_map_type <= 5) {
            m_ppsParam.slice_group_change_direction_flag = ns.readOneBit();
            m_ppsParam.slice_group_change_rate_minus1 = ns.readUev();
        }
        else if (6 == m_ppsParam.slice_group_map_type) {
            m_ppsParam.pic_size_in_map_units_minus1 = ns.readUev();
            for (auto idx = 0; idx <= m_ppsParam.pic_size_in_map_units_minus1; ++idx) {
                m_ppsParam.slice_group_id[idx] = ns.readUev();
            }
        }
    }

    m_ppsParam.num_ref_idx_l0_default_active_minus1 = ns.readUev();
    m_ppsParam.num_ref_idx_l1_default_active_minus1 = ns.readUev();

    m_ppsParam.weighted_pred_flag = ns.readOneBit();
    m_ppsParam.weighted_bipred_idc = ns.readNBits(2);

    m_ppsParam.pic_init_qp_minus26 = ns.readSev();
    m_ppsParam.pic_init_qs_minus26 = ns.readSev();
    m_ppsParam.chroma_qp_index_offset = ns.readSev();

    m_ppsParam.deblocking_filter_control_present_flag = ns.readOneBit();
    m_ppsParam.constrained_intra_pred_flag = ns.readOneBit();
    m_ppsParam.redundant_pic_cnt_present_flag = ns.readOneBit();

    if (!ns.isEnd()) {
        m_ppsParam.transform_8x8_mode_flag = ns.readOneBit();
        m_ppsParam.pic_scaling_matrix_present_flag = ns.readOneBit();

        if (0 != m_ppsParam.pic_scaling_matrix_present_flag) {
            for (auto idx = 0; idx < 6 + ((sps.m_spsParam.chroma_format_idc != 3) ? 2 : 6) * m_ppsParam.transform_8x8_mode_flag; ++idx) {
                m_ppsParam.pic_scaling_list_present_flag[idx] = ns.readOneBit();
                if (0 != m_ppsParam.pic_scaling_list_present_flag[idx]) {
                    if (idx < 6) {
                        NaluHelper::ParseScalingList(ns, m_ppsParam.scaling_list_4x4[idx], 16,
                            &(m_ppsParam.use_default_scaling_matrix_4x4_flag[idx]));
                    }
                    else {
                        NaluHelper::ParseScalingList(ns, m_ppsParam.scaling_list_8x8[idx - 6], 64,
                            &(m_ppsParam.use_default_scaling_matrix_8x8_flag[idx - 6]));
                    }
                }
            }
        }

        m_ppsParam.second_chroma_qp_index_offset = ns.readSev();
    }

    m_isValid = true;
    return true;
}

std::string NaluPPS::dumpString() const
{
    if (!m_isValid) {
        return "";
    }

    std::stringstream ss;
    ss << "PPS ========================" << std::endl;
    ss << "pic_parameter_set_id: " << m_ppsParam.pic_parameter_set_id << std::endl;
    ss << "seq_parameter_set_id: " << m_ppsParam.seq_parameter_set_id << std::endl;
    ss << "entropy_coding_mode_flag: " << static_cast<int>(m_ppsParam.entropy_coding_mode_flag) << std::endl;
    ss << "bottom_field_pic_order_in_frame_present_flag: " << static_cast<int>(m_ppsParam.bottom_field_pic_order_in_frame_present_flag) << std::endl;

    ss << "num_slice_groups_minus1: " << m_ppsParam.num_slice_groups_minus1 << std::endl;
    ss << "slice_group_map_type: " << m_ppsParam.slice_group_map_type << std::endl;

    ss << "run_length_minus1: ";
    for (auto iGroup = 0; iGroup <= m_ppsParam.num_slice_groups_minus1; ++iGroup) {
        ss << m_ppsParam.run_length_minus1[iGroup] << " ";
    }
    ss << std::endl;

    ss << "top_left: ";
    for (auto iGroup = 0; iGroup < m_ppsParam.num_slice_groups_minus1; ++iGroup) {
        ss << m_ppsParam.top_left[iGroup] << " ";
    }
    ss << std::endl;

    ss << "bottom_right: ";
    for (auto iGroup = 0; iGroup < m_ppsParam.num_slice_groups_minus1; ++iGroup) {
        ss << m_ppsParam.bottom_right[iGroup] << " ";
    }
    ss << std::endl;

    ss << "slice_group_change_direction_flag: " << static_cast<int>(m_ppsParam.slice_group_change_direction_flag) << std::endl;
    ss << "slice_group_change_rate_minus1: " << m_ppsParam.slice_group_change_rate_minus1 << std::endl;

    ss << "pic_size_in_map_units_minus1: " << m_ppsParam.pic_size_in_map_units_minus1 << std::endl;
    ss << "slice_group_id: ";
    for (auto idx = 0; idx <= m_ppsParam.pic_size_in_map_units_minus1; ++idx) {
        ss << static_cast<int>(m_ppsParam.slice_group_id[idx]) << " ";
    }
    ss << std::endl;

    ss << "num_ref_idx_l0_default_active_minus1: " << m_ppsParam.num_ref_idx_l0_default_active_minus1 << std::endl;
    ss << "num_ref_idx_l1_default_active_minus1: " << m_ppsParam.num_ref_idx_l1_default_active_minus1 << std::endl;

    ss << "weighted_pred_flag: " << static_cast<int>(m_ppsParam.weighted_pred_flag) << std::endl;
    ss << "weighted_bipred_idc: " << static_cast<int>(m_ppsParam.weighted_bipred_idc) << std::endl;

    ss << "pic_init_qp_minus26: " << m_ppsParam.pic_init_qp_minus26 << std::endl;
    ss << "pic_init_qs_minus26: " << m_ppsParam.pic_init_qs_minus26 << std::endl;
    ss << "chroma_qp_index_offset: " << m_ppsParam.chroma_qp_index_offset << std::endl;

    ss << "deblocking_filter_control_present_flag: " << static_cast<int>(m_ppsParam.deblocking_filter_control_present_flag) << std::endl;
    ss << "constrained_intra_pred_flag: " << static_cast<int>(m_ppsParam.constrained_intra_pred_flag) << std::endl;
    ss << "redundant_pic_cnt_present_flag: " << static_cast<int>(m_ppsParam.redundant_pic_cnt_present_flag) << std::endl;

    ss << "transform_8x8_mode_flag: " << static_cast<int>(m_ppsParam.transform_8x8_mode_flag) << std::endl;
    ss << "pic_scaling_matrix_present_flag: " << static_cast<int>(m_ppsParam.pic_scaling_matrix_present_flag) << std::endl;

    ss << "pic_scaling_list_present_flag: ";
    for (unsigned char val : m_ppsParam.pic_scaling_list_present_flag) {
        ss << static_cast<int>(val) << " ";
    }
    ss << std::endl;

    ss << "scaling_list_4x4: " << std::endl;
    for (const auto& idx : m_ppsParam.scaling_list_4x4) {
        for (int jdx : idx) {
            ss << jdx << " ";
        }
        ss << std::endl;
    }

    ss << "use_default_scaling_matrix_4x4_flag: ";
    for (int idx : m_ppsParam.use_default_scaling_matrix_4x4_flag) {
        ss << idx << " ";
    }
    ss << std::endl;

    ss << "scaling_list_8x8: " << std::endl;
    for (const auto& idx : m_ppsParam.scaling_list_8x8) {
        for (int jdx : idx) {
            ss << jdx << " ";
        }
        ss << std::endl;
    }

    ss << "use_default_scaling_matrix_8x8_flag: ";
    for (int idx : m_ppsParam.use_default_scaling_matrix_8x8_flag) {
        ss << idx << " ";
    }
    ss << std::endl;

    ss << "second_chroma_qp_index_offset: " << m_ppsParam.second_chroma_qp_index_offset << std::endl;

    return ss.str();
}

} // namespace AnnexB