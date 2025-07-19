#pragma once
#include <cstdint>

namespace Common {

// SAR扩展标识
constexpr int SAR_EXTEND = 255;

/**
 * @brief NALU类型
 */
typedef enum class NALU_TYPE : uint8_t {
    Unspecified     = 0x00,
    NonIDRSlice     = 0x01,
    PartitionA      = 0x02,
    PartitionB      = 0x03,
    PartitionC      = 0x04,
    IDRSlice        = 0x05,
    SEI             = 0x06,
    SPS             = 0x07,
    PPS             = 0x08,
    AUD             = 0x09,
    EndOfSequence   = 0x0a,
    EndOfStream     = 0x0b,
    FillerData      = 0x0c,
    SPSExt          = 0x0d,
    PrefixNalUnit   = 0x0e,
    SubsetSPS       = 0x0f,

} Nalu_t;

/**
 * @brief SPS Vui Hrd参数
 */
typedef struct SPS_VUI_HRD_PARAMETER_DATA_TYPE {
    uint32_t    cpb_cnt_minus1 { 0 };
    uint8_t     bit_rate_scale { 0 };
    uint8_t     cpb_size_scale { 0 };
    uint32_t    bit_rate_value_minus1[32] { 0 };
    uint32_t    cpb_size_value_minus1[32] { 0 };
    uint8_t     cbr_flag[32] { 0 };
    uint8_t     initial_cpb_removal_delay_length_minus1 { 0 };
    uint8_t     cpb_removal_delay_length_minus1 { 0 };
    uint8_t     dpb_output_delay_length_minus1 { 0 };
    uint8_t     time_offset_length { 0 };

} SPSVuiHrdParam_dt;

/**
 * @brief SPS Vui参数
 */
typedef struct SPS_VUI_PARAMETER_DATA_TYPE {
    //                  aspect ratio info
    uint8_t             aspect_ratio_info_present_flag { 0 };
    uint8_t             aspect_ratio_idc { 0 };
    uint16_t            sar_width { 0 };
    uint16_t            sar_height { 0 };
    uint8_t             overscan_info_present_flag { 0 };
    uint8_t             overscan_appropriate_flag { 0 };

    //                  video signal info
    uint8_t             video_signal_type_present_flag { 0 };
    uint8_t             video_format { 0 };
    uint8_t             video_full_range_flag { 0 };
    uint8_t             colour_description_present_flag { 0 };
    uint8_t             colour_primaries { 0 };
    uint8_t             transfer_characteristics { 0 };
    uint8_t             matrix_coefficients { 0 };

    uint8_t             chroma_loc_info_present_flag { 0 };
    uint32_t            chroma_sample_loc_type_top_field { 0 };
    uint32_t            chroma_sample_loc_type_bottom_field { 0 };

    //                  time info
    uint8_t             timing_info_present_flag { 0 };
    uint32_t            num_units_in_tick { 0 };
    uint32_t            time_scale { 0 };
    uint8_t             fixed_frame_rate_flag { 0 };

    uint8_t             nal_hrd_parameters_present_flag { 0 };
    uint8_t             vcl_hrd_parameters_present_flag { 0 };
    uint8_t             low_delay_hrd_flag { 0 };
    uint8_t             pic_struct_present_flag { 0 };
    uint8_t             bitstream_restriction_flag { 0 };
    uint8_t             motion_vectors_over_pic_boundaries_flag { 0 };
    uint32_t            max_bytes_per_pic_denom { 0 };
    uint32_t            max_bits_per_mb_denom { 0 };
    uint32_t            log2_max_mv_length_horizontal { 0 };
    uint32_t            log2_max_mv_length_vertical { 0 };
    uint32_t            num_reorder_frames { 0 };
    uint32_t            max_dec_frame_buffering { 0 };
    SPSVuiHrdParam_dt   hrd_nal;
    SPSVuiHrdParam_dt   hrd_vcl;

} SPSVuiParam_dt;

/**
 * @brief SPS参数
 */
typedef struct SPS_PARAMETER_DATA_TYPE {
    uint8_t         profile_idc { 0 };

    //              constraint_set0_flag - constraint_set7_flag
    uint8_t         constraint_set_flag { 0 };

    uint8_t         level_idc { 0 };
    uint32_t        seq_parameter_set_id { 0 };

    //              chroma_format_idc, 默认值为1，代表YUV420
    uint32_t        chroma_format_idc { 1 };

    uint8_t         separate_colour_plane_flag { 0 };
    uint32_t        bit_depth_luma_minus8 { 0 };
    uint32_t        bit_depth_chroma_minus8 { 0 };
    uint8_t         qpprime_y_zero_transform_bypass_flag { 0 };
    uint8_t         seq_scaling_matrix_present_flag { 0 };
    uint8_t         seq_scaling_list_present_flag[12] { 0 };
    int             scaling_list_4x4[6][16] { 0 };
    int             use_default_scaling_matrix_4x4_flag[6] { 0 };
    int             scaling_list_8x8[6][64] { 0 };
    int             use_default_scaling_matrix_8x8_flag[6] { 0 };
    uint32_t        log2_max_frame_num_minus4 { 0 };
    uint32_t        pic_order_cnt_type { 0 };
    uint32_t        log2_max_pic_order_cnt_lsb_minus4 { 0 };
    uint8_t         delta_pic_order_always_zero_flag { 0 };
    int             offset_for_non_ref_pic { 0 };
    int             offset_for_top_to_bottom_field { 0 };
    uint32_t        num_ref_frames_in_pic_order_cnt_cycle { 0 };
    int             offset_for_ref_frame[256] { 0 };
    uint32_t        max_num_ref_frames { 0 };
    uint8_t         gaps_in_frame_num_value_allowed_flag { 0 };
    uint32_t        pic_width_in_mbs_minus1 { 0 };
    uint32_t        pic_height_in_map_units_minus1 { 0 };
    uint8_t         frame_mbs_only_flag { 0 };
    uint8_t         mb_adaptive_frame_field_flag { 0 };
    uint8_t         direct_8x8_inference_flag { 0 };
    uint8_t         frame_cropping_flag { 0 };
    uint32_t        frame_crop_left_offset { 0 };
    uint32_t        frame_crop_right_offset { 0 };
    uint32_t        frame_crop_top_offset { 0 };
    uint32_t        frame_crop_bottom_offset { 0 };

    //              Video Usability Information
    uint8_t         vui_parameters_present_flag { 0 };
    SPSVuiParam_dt  vui;

} SPSParam_dt;

/**
 * @brief PPS参数
 */
typedef struct PPS_PARAMETER_DATA_TYPE {
    uint32_t        pic_parameter_set_id { 0 };
    uint32_t        seq_parameter_set_id { 0 };
    uint8_t         entropy_coding_mode_flag { 0 };
    uint8_t         bottom_field_pic_order_in_frame_present_flag { 0 };
    uint32_t        num_slice_groups_minus1 { 0 };
    uint32_t        slice_group_map_type { 0 };
    uint32_t        run_length_minus1[8] { 0 };
    uint32_t        top_left[8] { 0 };
    uint32_t        bottom_right[8] { 0 };
    uint8_t         slice_group_change_direction_flag { 0 };
    uint32_t        slice_group_change_rate_minus1 { 0 };
    uint32_t        pic_size_in_map_units_minus1 { 0 };
    uint8_t         slice_group_id[256] { 0 };
    uint32_t        num_ref_idx_l0_default_active_minus1 { 0 };
    uint32_t        num_ref_idx_l1_default_active_minus1 { 0 };
    uint8_t         weighted_pred_flag { 0 };
    uint16_t        weighted_bipred_idc { 0 };
    int             pic_init_qp_minus26 { 0 };
    int             pic_init_qs_minus26 { 0 };
    int             chroma_qp_index_offset { 0 };
    uint8_t         deblocking_filter_control_present_flag { 0 };
    uint8_t         constrained_intra_pred_flag { 0 };
    uint8_t         redundant_pic_cnt_present_flag { 0 };
    uint8_t         transform_8x8_mode_flag { 0 };
    uint8_t         pic_scaling_matrix_present_flag { 0 };
    uint8_t         pic_scaling_list_present_flag[12] { 0 };
    int             scaling_list_4x4[6][16] { 0 };
    int             use_default_scaling_matrix_4x4_flag[6] { 0 };
    int             scaling_list_8x8[6][64] { 0 };
    int             use_default_scaling_matrix_8x8_flag[6] { 0 };
    int             second_chroma_qp_index_offset { 0 };

} PPSParam_dt;

}; // namespace Common
