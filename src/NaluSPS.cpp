#include <set>
#include <sstream>
#include <iostream>
#include "Utils/NaluHelper.h"
#include "AnnexB/NaluStream.h"
#include "AnnexB/NaluSPS.h"
using Common::Nalu_t;
using Common::SPSVuiParam_dt;
using Common::SPSVuiHrdParam_dt;
using Utils::NaluHelper;

namespace AnnexB {

// 高画质idc id
static std::set<uint32_t> HighProfileIdcSet = {
    44, 83, 86, 100, 110, 118, 122, 128, 134, 135, 138, 139, 244
};

bool NaluSPS::parse(const std::vector<uint8_t>& buffer) {
    if (buffer.empty()) {
        std::cerr << "Parse nalu sps error. input param invalid." << std::endl;
        return false;
    }

    NaluStream ns(buffer.data(), buffer.size());

    // 读取nalu类型
    int head = ns.readNBits(8);
    auto type = static_cast<Nalu_t>(head & 0x1f);
    if (Nalu_t::SPS != type) {
        std::cerr << "Parse nalu sps error. nalu type invalid." << std::endl;
        return false;
    }

    // profile_idc
    m_spsParam.profile_idc = ns.readNBits(8);

    // constraint_setX_flags
    m_spsParam.constraint_set_flag = ns.readNBits(8);

    // level_idc
    m_spsParam.level_idc = ns.readNBits(8);

    // seq_parameter_set_id
    m_spsParam.seq_parameter_set_id = ns.readUev();

    // high profile idc
    if (HighProfileIdcSet.end() != HighProfileIdcSet.find(m_spsParam.profile_idc)) {
        // chroma_format_idc
        m_spsParam.chroma_format_idc = ns.readUev();

        // 3代表YUV 4:4:4
        if (3 == m_spsParam.chroma_format_idc) {
            // separate_colour_plane_flag
            m_spsParam.separate_colour_plane_flag = ns.readOneBit();
        }

        // bit_depth_luma_minus8
        m_spsParam.bit_depth_luma_minus8 = ns.readUev();

        // bit_depth_chroma_minus8
        m_spsParam.bit_depth_chroma_minus8 = ns.readUev();

        // qpprime_y_zero_transform_bypass_flag
        m_spsParam.qpprime_y_zero_transform_bypass_flag = ns.readOneBit();

        // seq_scaling_matrix_present_flag
        m_spsParam.seq_scaling_matrix_present_flag = ns.readOneBit();

        if (0 != m_spsParam.seq_scaling_matrix_present_flag) {
            for (int idx = 0; idx < ((3 != m_spsParam.chroma_format_idc) ? 8 : 12); ++idx) {
                // seq_scaling_list_present_flag
                m_spsParam.seq_scaling_list_present_flag[idx] = ns.readUev();

                if (0 != m_spsParam.seq_scaling_list_present_flag[idx]) {
                    if (idx < 6) {
                        // ScalingList4x4/UseDefaultScalingMatrix4x4Flag
                        NaluHelper::ParseScalingList(ns, m_spsParam.scaling_list_4x4[idx], 16,
                            &(m_spsParam.use_default_scaling_matrix_4x4_flag[idx]));
                    }
                    else {
                        // ScalingList8x8/UseDefaultScalingMatrix8x8Flag
                        NaluHelper::ParseScalingList(ns, m_spsParam.scaling_list_8x8[idx - 6], 64,
                            &(m_spsParam.use_default_scaling_matrix_8x8_flag[idx - 6]));
                    }
                }
            }
        }
    }

    // log2_max_frame_num_minus4
    m_spsParam.log2_max_frame_num_minus4 = ns.readUev();

    // pic_order_cnt_type
    m_spsParam.pic_order_cnt_type = ns.readUev();

    if (0 == m_spsParam.pic_order_cnt_type) {
        // log2_max_pic_order_cnt_lsb_minus4
        m_spsParam.log2_max_pic_order_cnt_lsb_minus4 = ns.readUev();
    }
    else if (1 == m_spsParam.pic_order_cnt_type) {
        // delta_pic_order_always_zero_flag
        m_spsParam.delta_pic_order_always_zero_flag = ns.readOneBit();

        // offset_for_non_ref_pic
        m_spsParam.offset_for_non_ref_pic = ns.readSev();

        // offset_for_top_to_bottom_field
        m_spsParam.offset_for_top_to_bottom_field = ns.readSev();

        // num_ref_frames_in_pic_order_cnt_cycle
        m_spsParam.num_ref_frames_in_pic_order_cnt_cycle = ns.readUev();

        // offset_for_ref_frame
        for (int idx = 0; idx < m_spsParam.num_ref_frames_in_pic_order_cnt_cycle; ++idx) {
            m_spsParam.offset_for_ref_frame[idx] = ns.readSev();
        }
    }

    // max_num_ref_frames
    m_spsParam.max_num_ref_frames = ns.readUev();

    //  gaps_in_frame_num_value_allowed_flag
    m_spsParam.gaps_in_frame_num_value_allowed_flag = ns.readOneBit();

    // pic_width_in_mbs_minus1
    m_spsParam.pic_width_in_mbs_minus1 = ns.readUev();

    // pic_height_in_map_units_minus1
    m_spsParam.pic_height_in_map_units_minus1 = ns.readUev();

    // frame_mbs_only_flag
    m_spsParam.frame_mbs_only_flag = ns.readOneBit();

    // mb_adaptive_frame_field_flag
    if (0 == m_spsParam.frame_mbs_only_flag) {
        m_spsParam.mb_adaptive_frame_field_flag = ns.readOneBit();
    }

    // direct_8x8_inference_flag
    m_spsParam.direct_8x8_inference_flag = ns.readOneBit();

    // frame_cropping_flag
    m_spsParam.frame_cropping_flag = ns.readOneBit();

    if (0 != m_spsParam.frame_cropping_flag) {
        // frame_crop_left_offset
        m_spsParam.frame_crop_left_offset = ns.readUev();

        // frame_crop_right_offset
        m_spsParam.frame_crop_right_offset = ns.readUev();

        // frame_crop_top_offset
        m_spsParam.frame_crop_top_offset = ns.readUev();

        // frame_crop_bottom_offset
        m_spsParam.frame_crop_bottom_offset = ns.readUev();
    }

    // vui_parameters_present_flag
    m_spsParam.vui_parameters_present_flag = ns.readOneBit();

    // vui
    if (0 != m_spsParam.vui_parameters_present_flag) {
        NaluHelper::ParseVuiParam(ns, m_spsParam.vui);
    }

    return true;
}

std::string NaluSPS::dumpString() const {
    std::stringstream ss;
    ss << "SPS ========================" << std::endl;
    ss << "profile_idc: " << static_cast<int>(m_spsParam.profile_idc) << std::endl;

    for (int idx = 0; idx < 8; ++idx) {
        ss << "constraint_set" << idx << "_flag: " << static_cast<int>((m_spsParam.constraint_set_flag >> (7 - idx)) & 0x01) << std::endl;
    }

    ss << "level_idc: " << static_cast<int>(m_spsParam.level_idc) << std::endl;
    ss << "seq_parameter_set_id: " << m_spsParam.seq_parameter_set_id << std::endl;

    if (HighProfileIdcSet.end() != HighProfileIdcSet.find(m_spsParam.profile_idc)) {
        ss << "chroma_format_idc: " << m_spsParam.chroma_format_idc << std::endl;
        if (3 == m_spsParam.chroma_format_idc) {
            ss << "separate_colour_plane_flag: " << static_cast<int>(m_spsParam.separate_colour_plane_flag) << std::endl;
        }

        ss << "bit_depth_luma_minus8: " << m_spsParam.bit_depth_luma_minus8 << std::endl;
        ss << "bit_depth_chroma_minus8: " << m_spsParam.bit_depth_chroma_minus8 << std::endl;

        ss << "qpprime_y_zero_transform_bypass_flag: " << static_cast<int>(m_spsParam.qpprime_y_zero_transform_bypass_flag) << std::endl;
        ss << "seq_scaling_matrix_present_flag: " << static_cast<int>(m_spsParam.seq_scaling_matrix_present_flag) << std::endl;

        if (0 != m_spsParam.seq_scaling_matrix_present_flag) {
            ss << "seq_scaling_list_present_flag: ";
            for (auto idx : m_spsParam.seq_scaling_list_present_flag) {
                ss << static_cast<int>(idx) << " ";
            }
            ss << std::endl;

            ss << "scaling_list_4x4: " << std::endl;
            for (const auto& idx : m_spsParam.scaling_list_4x4) {
                for (int jdx : idx) {
                    ss << jdx << " ";
                }
                ss << std::endl;
            }

            ss << "use_default_scaling_matrix_4x4_flag: ";
            for (int idx : m_spsParam.use_default_scaling_matrix_4x4_flag) {
                ss << idx << " ";
            }
            ss << std::endl;

            ss << "scaling_list_8x8: " << std::endl;
            for (const auto& idx : m_spsParam.scaling_list_8x8) {
                for (int jdx : idx) {
                    ss << jdx << " ";
                }
                ss << std::endl;
            }

            ss << "use_default_scaling_matrix_8x8_flag: ";
            for (int idx : m_spsParam.use_default_scaling_matrix_8x8_flag) {
                ss << idx << " ";
            }
            ss << std::endl;
        }
    }

    ss << "log2_max_frame_num_minus4: " << m_spsParam.log2_max_frame_num_minus4 << std::endl;
    ss << "pic_order_cnt_type: " << m_spsParam.pic_order_cnt_type << std::endl;

    if (0 == m_spsParam.pic_order_cnt_type) {
        ss << "log2_max_pic_order_cnt_lsb_minus4: " << m_spsParam.log2_max_pic_order_cnt_lsb_minus4 << std::endl;
    }
    else if (1 == m_spsParam.pic_order_cnt_type) {
        ss << "delta_pic_order_always_zero_flag: " << static_cast<int>(m_spsParam.delta_pic_order_always_zero_flag) << std::endl;
        ss << "offset_for_non_ref_pic: " << m_spsParam.offset_for_non_ref_pic << std::endl;
        ss << "offset_for_top_to_bottom_field: " << m_spsParam.offset_for_top_to_bottom_field << std::endl;
        ss << "num_ref_frames_in_pic_order_cnt_cycle: " << m_spsParam.num_ref_frames_in_pic_order_cnt_cycle << std::endl;

        ss << "offset_for_ref_frame: ";
        for (int idx : m_spsParam.offset_for_ref_frame) {
            ss << idx << " ";
        }
        ss << std::endl;
    }

    ss << "max_num_ref_frames: " << m_spsParam.max_num_ref_frames << std::endl;
    ss << "gaps_in_frame_num_value_allowed_flag: " << static_cast<int>(m_spsParam.gaps_in_frame_num_value_allowed_flag) << std::endl;
    ss << "pic_width_in_mbs_minus1: " << m_spsParam.pic_width_in_mbs_minus1 << std::endl;
    ss << "pic_height_in_map_units_minus1: " << m_spsParam.pic_height_in_map_units_minus1 << std::endl;
    ss << "frame_mbs_only_flag: " << static_cast<int>(m_spsParam.frame_mbs_only_flag) << std::endl;

    if (0 == m_spsParam.frame_mbs_only_flag) {
        ss << "mb_adaptive_frame_field_flag: " << static_cast<int>(m_spsParam.mb_adaptive_frame_field_flag) << std::endl;
    }

    ss << "direct_8x8_inference_flag: " << static_cast<int>(m_spsParam.direct_8x8_inference_flag) << std::endl;
    ss << "frame_cropping_flag: " << static_cast<int>(m_spsParam.frame_cropping_flag) << std::endl;

    if (m_spsParam.frame_cropping_flag) {
        ss << "frame_crop_left_offset: " << m_spsParam.frame_crop_left_offset << std::endl;
        ss << "frame_crop_right_offset: " << m_spsParam.frame_crop_right_offset << std::endl;
        ss << "frame_crop_top_offset: " << m_spsParam.frame_crop_top_offset << std::endl;
        ss << "frame_crop_bottom_offset: " << m_spsParam.frame_crop_bottom_offset << std::endl;
    }

    ss << "vui_parameters_present_flag: " << static_cast<int>(m_spsParam.vui_parameters_present_flag) << std::endl;

    if (0 != m_spsParam.vui_parameters_present_flag) {
        ss << "VUI -------------------" << std::endl;

        ss << "aspect_ratio_info_present_flag: " << static_cast<int>(m_spsParam.vui.aspect_ratio_info_present_flag) << std::endl;

        if (0 != m_spsParam.vui.aspect_ratio_info_present_flag) {
            ss << "aspect_ratio_idc: " << m_spsParam.vui.aspect_ratio_idc << std::endl;

            if(Common::SAR_EXTEND == m_spsParam.vui.aspect_ratio_idc) {
                ss << "sar_width: " << m_spsParam.vui.sar_width << std::endl;
                ss << "sar_height: " << m_spsParam.vui.sar_height << std::endl;
            }
        }

        ss << "overscan_info_present_flag: " << static_cast<int>(m_spsParam.vui.overscan_info_present_flag) << std::endl;

        if (0 != m_spsParam.vui.overscan_info_present_flag) {
            ss << "overscan_appropriate_flag: " << static_cast<int>(m_spsParam.vui.overscan_appropriate_flag) << std::endl;
        }

        ss << "video_signal_type_present_flag: " << static_cast<int>(m_spsParam.vui.video_signal_type_present_flag) << std::endl;

        if (0 != m_spsParam.vui.video_signal_type_present_flag) {
            ss << "video_format: " << m_spsParam.vui.video_format << std::endl;
            ss << "video_full_range_flag: " << static_cast<int>(m_spsParam.vui.video_full_range_flag) << std::endl;
            ss << "colour_description_present_flag: " << static_cast<int>(m_spsParam.vui.colour_description_present_flag) << std::endl;

            if (0 != m_spsParam.vui.colour_description_present_flag) {
                ss << "colour_primaries: " << m_spsParam.vui.colour_primaries << std::endl;
                ss << "transfer_characteristics: " << m_spsParam.vui.transfer_characteristics << std::endl;
                ss << "matrix_coefficients: " << m_spsParam.vui.matrix_coefficients << std::endl;
            }
        }

        ss << "chroma_loc_info_present_flag: " << static_cast<int>(m_spsParam.vui.chroma_loc_info_present_flag) << std::endl;

        if (0 != m_spsParam.vui.chroma_loc_info_present_flag) {
            ss << "chroma_sample_loc_type_top_field: " << m_spsParam.vui.chroma_sample_loc_type_top_field << std::endl;
            ss << "chroma_sample_loc_type_bottom_field: " << m_spsParam.vui.chroma_sample_loc_type_bottom_field << std::endl;
        }

        ss << "timing_info_present_flag: " << static_cast<int>(m_spsParam.vui.timing_info_present_flag) << std::endl;

        if (0 != m_spsParam.vui.timing_info_present_flag) {
            ss << "num_units_in_tick: " << m_spsParam.vui.num_units_in_tick << std::endl;
            ss << "time_scale: " << m_spsParam.vui.time_scale << std::endl;
            ss << "fixed_frame_rate_flag: " << static_cast<int>(m_spsParam.vui.fixed_frame_rate_flag) << std::endl;
        }

        ss << "nal_hrd_parameters_present_flag: " << static_cast<int>(m_spsParam.vui.nal_hrd_parameters_present_flag) << std::endl;

        if (0 != m_spsParam.vui.nal_hrd_parameters_present_flag) {
            ss << "hrd_nal -------------------" << std::endl;
            ss << "cpb_cnt_minus1: " << m_spsParam.vui.hrd_nal.cpb_cnt_minus1 << std::endl;
            ss << "bit_rate_scale: " << static_cast<int>(m_spsParam.vui.hrd_nal.bit_rate_scale) << std::endl;
            ss << "cpb_size_scale: " << static_cast<int>(m_spsParam.vui.hrd_nal.cpb_size_scale) << std::endl;

            ss << "bit_rate_value_minus1: ";
            for (unsigned int idx : m_spsParam.vui.hrd_nal.bit_rate_value_minus1) {
                ss << idx << " ";
            }
            ss << std::endl;

            ss << "cpb_size_value_minus1: ";
            for (unsigned int idx : m_spsParam.vui.hrd_nal.cpb_size_value_minus1) {
                ss << idx << " ";
            }
            ss << std::endl;

            ss << "cbr_flag: ";
            for (unsigned char idx : m_spsParam.vui.hrd_nal.cbr_flag) {
                ss << static_cast<int>(idx) << " ";
            }
            ss << std::endl;

            ss << "initial_cpb_removal_delay_length_minus1: " << static_cast<int>(m_spsParam.vui.hrd_nal.initial_cpb_removal_delay_length_minus1) << std::endl;
            ss << "cpb_removal_delay_length_minus1: " << static_cast<int>(m_spsParam.vui.hrd_nal.cpb_removal_delay_length_minus1) << std::endl;
            ss << "dpb_output_delay_length_minus1: " << static_cast<int>(m_spsParam.vui.hrd_nal.dpb_output_delay_length_minus1) << std::endl;
            ss << "time_offset_length: " << static_cast<int>(m_spsParam.vui.hrd_nal.time_offset_length) << std::endl;
        }

        ss << "vcl_hrd_parameters_present_flag: " << static_cast<int>(m_spsParam.vui.vcl_hrd_parameters_present_flag) << std::endl;

        if (0 != m_spsParam.vui.vcl_hrd_parameters_present_flag) {
            ss << "hrd_vcl -------------------" << std::endl;
            ss << "cpb_cnt_minus1: " << m_spsParam.vui.hrd_vcl.cpb_cnt_minus1 << std::endl;
            ss << "bit_rate_scale: " << static_cast<int>(m_spsParam.vui.hrd_vcl.bit_rate_scale) << std::endl;
            ss << "cpb_size_scale: " << static_cast<int>(m_spsParam.vui.hrd_vcl.cpb_size_scale) << std::endl;

            ss << "bit_rate_value_minus1: ";
            for (unsigned int idx : m_spsParam.vui.hrd_vcl.bit_rate_value_minus1) {
                ss << idx << " ";
            }
            ss << std::endl;

            ss << "cpb_size_value_minus1: ";
            for (unsigned int idx : m_spsParam.vui.hrd_vcl.cpb_size_value_minus1) {
                ss << idx << " ";
            }
            ss << std::endl;

            ss << "cbr_flag: ";
            for (unsigned char idx : m_spsParam.vui.hrd_vcl.cbr_flag) {
                ss << static_cast<int>(idx) << " ";
            }
            ss << std::endl;

            ss << "initial_cpb_removal_delay_length_minus1: " << static_cast<int>(m_spsParam.vui.hrd_vcl.initial_cpb_removal_delay_length_minus1) << std::endl;
            ss << "cpb_removal_delay_length_minus1: " << static_cast<int>(m_spsParam.vui.hrd_vcl.cpb_removal_delay_length_minus1) << std::endl;
            ss << "dpb_output_delay_length_minus1: " << static_cast<int>(m_spsParam.vui.hrd_vcl.dpb_output_delay_length_minus1) << std::endl;
            ss << "time_offset_length: " << static_cast<int>(m_spsParam.vui.hrd_vcl.time_offset_length) << std::endl;
        }

        if(0 != m_spsParam.vui.nal_hrd_parameters_present_flag || 0 != m_spsParam.vui.vcl_hrd_parameters_present_flag) {
            ss << "low_delay_hrd_flag: " << static_cast<int>(m_spsParam.vui.low_delay_hrd_flag) << std::endl;
        }

        ss << "pic_struct_present_flag: " << static_cast<int>(m_spsParam.vui.pic_struct_present_flag) << std::endl;
        ss << "bitstream_restriction_flag: " << static_cast<int>(m_spsParam.vui.bitstream_restriction_flag) << std::endl;

        if (0 != m_spsParam.vui.bitstream_restriction_flag) {
            ss << "motion_vectors_over_pic_boundaries_flag: " << static_cast<int>(m_spsParam.vui.motion_vectors_over_pic_boundaries_flag) << std::endl;
            ss << "max_bytes_per_pic_denom: " << m_spsParam.vui.max_bytes_per_pic_denom << std::endl;
            ss << "max_bits_per_mb_denom: " << m_spsParam.vui.max_bits_per_mb_denom << std::endl;
            ss << "log2_max_mv_length_horizontal: " << m_spsParam.vui.log2_max_mv_length_horizontal << std::endl;
            ss << "log2_max_mv_length_vertical: " << m_spsParam.vui.log2_max_mv_length_vertical << std::endl;
            ss << "num_reorder_frames: " << m_spsParam.vui.num_reorder_frames << std::endl;
            ss << "max_dec_frame_buffering: " << m_spsParam.vui.max_dec_frame_buffering << std::endl;
        }
    }

    return ss.str();
}

std::string NaluSPS::dumpResolution() const {
    auto width = (m_spsParam.pic_width_in_mbs_minus1 + 1) * 16;
    auto height = (m_spsParam.pic_height_in_map_units_minus1 + 1) * 16;

    // 场编码，场编码会将一帧数据按照单数/双数行分为两场
    if (0 == m_spsParam.frame_mbs_only_flag) {
        height *= 2;
    }

    // 编码时使用了补充像素，解码时需要去除
    if (1 == m_spsParam.frame_cropping_flag) {
        int crop_unit_x = 1;
        int crop_unit_y = 1;

        if (0 == m_spsParam.chroma_format_idc) {
            // 颜色格式为单色格式
            crop_unit_x = 1;
            crop_unit_y = (0 == m_spsParam.frame_mbs_only_flag ? 2 : 1);
        }
        else if (1 == m_spsParam.chroma_format_idc) {
            // 颜色格式为YUV 4:2:0
            crop_unit_x = 2;
            crop_unit_y = 2 * (0 == m_spsParam.frame_mbs_only_flag ? 2 : 1);
        }
        else if (2 == m_spsParam.chroma_format_idc) {
            // 颜色格式为YUV 4:2:2
            crop_unit_x = 2;
            crop_unit_y = 1 * (0 == m_spsParam.frame_mbs_only_flag ? 2 : 1);
        }
        else if (3 == m_spsParam.chroma_format_idc) {
            // 颜色格式为YUV 4:4:4
            crop_unit_x = 1;
            crop_unit_y = 1 * (0 == m_spsParam.frame_mbs_only_flag ? 2 : 1);
        }

        // 实际宽度需要减去补充系数 * 补充像素数量
        width -= crop_unit_x * (m_spsParam.frame_crop_left_offset + m_spsParam.frame_crop_right_offset);

        // 实际高度需要减去补充系数 * 补充像素数量
        height -= crop_unit_y * (m_spsParam.frame_crop_top_offset + m_spsParam.frame_crop_bottom_offset);
    }

    std::stringstream ss;
    ss << "RESOLUTION ========================" << std::endl;
    ss << "width: " << width << ", height: " << height << std::endl;
    return ss.str();
}

} // namespace AnnexB