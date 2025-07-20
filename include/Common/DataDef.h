#pragma once
#include <vector>
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
    DepthParamSet   = 0x10,
    AuxiliarySlice  = 0x13,
    CodedSliceExt   = 0x14,
    CodedSliceExt2  = 0x15,

} Nalu_t;

/**
 * @brief NALU Slice类型
 */
typedef enum class SLICE_TYPE : uint8_t {
    P_SLICE         = 0,
    B_SLICE         = 1,
    I_SLICE         = 2,
    SP_SLICE        = 3,
    SI_SLICE        = 4,

} Slice_t;

/**
 * @brief NALU SEI载荷类型
 */
typedef enum class NALU_SEI_PAYLOAD_TYPE : uint8_t {
    buffering_period                            = 0,
    picture_timing                              = 1,
    pain_scan_rect                              = 2,
    filler_payload                              = 3,
    user_data_registered_itu_t_t35              = 4,
    user_data_unregistered                      = 5,
    recovery_point                              = 6,
    dec_ref_pic_marking_repetition              = 7,
    spare_pic                                   = 8,
    scene_info                                  = 9,
    sub_seq_info                                = 10,
    sub_seq_layer_characteristics               = 11,
    sub_seq_characteristics                     = 12,
    full_frame_freeze                           = 13,
    full_frame_freeze_release                   = 14,
    full_frame_snapshot                         = 15,
    progressive_refinement_segment_start        = 16,
    progressive_refinement_segment_end          = 17,
    motion_constrained_slice_group_set          = 18,
    film_grain_characteristics                  = 19,
    deblocking_filter_display_preference        = 20,
    stereo_video_info                           = 21,
    post_filter_hint                            = 22,
    tone_mapping_info                           = 23,
    scalability_info                            = 24,
    sub_pic_scalable_layers                     = 25,
    non_required_layer_rep                      = 26,
    priority_layer_info                         = 27,
    layers_not_present                          = 28,
    layer_dependency_change                     = 29,
    scalable_nesting                            = 30,
    base_layer_temporal_hrd                     = 31,
    quality_layer_integrity_check               = 32,
    redundant_pic_property                      = 33,
    tl0_dep_rep_index                           = 34,
    tl_switching_point                          = 35,
    parallel_decoding_info                      = 36,
    mvc_scalable_nesting                        = 37,
    view_scalability_info                       = 38,
    multiple_view_scene_info                    = 39,
    multiview_acquisition_info                  = 40,
    non_required_view_components                = 41,
    view_dependency_change                      = 42,
    operation_points_not_present                = 43,
    base_view_temporal_hrd                      = 44,
    frame_packing_arrangement                   = 45,
    multiview_view_position                     = 46,
    display_orientation                         = 47,
    mvcd_scalable_nesting                       = 48,
    mvcd_view_scalability_info                  = 49,
    depth_representation_info                   = 50,
    three_dimensional_reference_displays_info   = 51,
    depth_timing                                = 52,
    depth_sampling_info                         = 53,
    constrained_depth_parameter_set_identifier  = 54,
    green_metadata                              = 56,
    mastering_display_colour_volume             = 137,
    colour_remapping_info                       = 142,
    content_light_level_info                    = 144,
    alternative_transfer_characteristics        = 147,
    ambient_viewing_environment                 = 148,
    content_colour_volume                       = 149,
    equirectangular_projection                  = 150,
    cubemap_projection                          = 151,
    sphere_rotation                             = 154,
    regionwise_packing                          = 155,
    omni_viewport                               = 156,
    alternative_depth_info                      = 181,
    sei_manifestation                           = 200,
    sei_prefix_indication                       = 201,
    annotated_regions                           = 202,
    shutter_interval_info                       = 205,
    nn_post_filter_characteristics              = 210,
    nn_post_filter_activation                   = 211,
    phase_indication                            = 212,
    reserved_sei_message                        = 255,

} NaluSeiPt_t;

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
    uint32_t                pic_parameter_set_id { 0 };
    uint32_t                seq_parameter_set_id { 0 };
    uint8_t                 entropy_coding_mode_flag { 0 };
    uint8_t                 bottom_field_pic_order_in_frame_present_flag { 0 };
    uint32_t                num_slice_groups_minus1 { 0 };
    uint32_t                slice_group_map_type { 0 };
    uint32_t                run_length_minus1[8] { 0 };
    uint32_t                top_left[8] { 0 };
    uint32_t                bottom_right[8] { 0 };
    uint8_t                 slice_group_change_direction_flag { 0 };
    uint32_t                slice_group_change_rate_minus1 { 0 };
    uint32_t                pic_size_in_map_units_minus1 { 0 };
    uint8_t                 slice_group_id[256] { 0 };
    uint32_t                num_ref_idx_l0_default_active_minus1 { 0 };
    uint32_t                num_ref_idx_l1_default_active_minus1 { 0 };
    uint8_t                 weighted_pred_flag { 0 };
    uint16_t                weighted_bipred_idc { 0 };
    int                     pic_init_qp_minus26 { 0 };
    int                     pic_init_qs_minus26 { 0 };
    int                     chroma_qp_index_offset { 0 };
    uint8_t                 deblocking_filter_control_present_flag { 0 };
    uint8_t                 constrained_intra_pred_flag { 0 };
    uint8_t                 redundant_pic_cnt_present_flag { 0 };
    uint8_t                 transform_8x8_mode_flag { 0 };
    uint8_t                 pic_scaling_matrix_present_flag { 0 };
    uint8_t                 pic_scaling_list_present_flag[12] { 0 };
    int                     scaling_list_4x4[6][16] { 0 };
    int                     use_default_scaling_matrix_4x4_flag[6] { 0 };
    int                     scaling_list_8x8[6][64] { 0 };
    int                     use_default_scaling_matrix_8x8_flag[6] { 0 };
    int                     second_chroma_qp_index_offset { 0 };

} PPSParam_dt;

/**
 * @brief SEI参数
 */
typedef struct SEI_PARAMETER_DATA_TYPE {
    uint32_t                payload_type { 0 };
    uint32_t                payload_size { 0 };
    std::vector<uint8_t>    payload_data;

} SEIParam_dt;

/**
 * @brief Slice Header ref_pic_list_modification 参数
 */
typedef struct REF_PIC_LIST_MODIFICATION_PARAMETER_DATA_TYPE {
    uint8_t                 ref_pic_list_modification_flag_l0 { 0 };
    uint32_t                modification_of_pic_nums_idc { 0 };
    uint32_t                abs_diff_pic_num_minus1 { 0 };
    uint32_t                long_term_pic_num { 0 };
    uint8_t                 ref_pic_list_modification_flag_l1 { 0 };

} RefPicListModParam_dt;

/**
 * @brief Slice Header pred_weight_table 参数
 */
typedef struct PRED_WEIGHT_TABLE_PARAMETER_DATA_TYPE {
    uint32_t                luma_log2_weight_denom { 0 };
    uint32_t                chroma_log2_weight_denom { 0 };

    uint8_t                 luma_weight_l0_flag { 0 };
    int32_t                 luma_weight_l0[32] { 0 };
    int32_t                 luma_offset_l0[32] { 0 };

    uint8_t                 chroma_weight_l0_flag { 0 };
    int32_t                 chroma_weight_l0[32][2] { 0 };
    int32_t                 chroma_offset_l0[32][2] { 0 };

    uint8_t                 luma_weight_l1_flag { 0 };
    int32_t                 luma_weight_l1[32] { 0 };
    int32_t                 luma_offset_l1[32] { 0 };

    uint8_t                 chroma_weight_l1_flag { 0 };
    int32_t                 chroma_weight_l1[32][2] { 0 };
    int32_t                 chroma_offset_l1[32][2] { 0 };

} PredWeightTableParam_dt;

/**
 * @brief Slice Header dec_ref_pic_marking 参数
 */
typedef struct DEC_REF_PIC_MARKING_PARAMETER_DATA_TYPE {
    uint8_t                 no_output_of_prior_pics_flag { 0 };
    uint8_t                 long_term_reference_flag { 0 };
    uint8_t                 adaptive_ref_pic_marking_mode_flag { 0 };
    uint32_t                memory_management_control_operation { 0 };
    uint32_t                difference_of_pic_nums_minus1 { 0 };
    uint32_t                long_term_pic_num { 0 };
    uint32_t                long_term_frame_idx { 0 };
    uint32_t                max_long_term_frame_idx_plus1 { 0 };

} DecRefPicMarkParam_dt;

/**
 * @brief Slice Header参数
 */
typedef struct SLICE_HEADER_PARAMETER_DATA_TYPE {
    uint32_t                first_mb_in_slice { 0 };
    uint32_t                slice_type { 0 };
    uint32_t                pic_parameter_set_id { 0 };
    uint16_t                colour_plane_id { 0 };
    uint32_t                frame_num { 0 };
    uint8_t                 field_pic_flag { 0 };
    uint8_t                 bottom_field_flag { 0 };
    uint32_t                idr_pic_id { 0 };
    uint32_t                pic_order_cnt_lsb { 0 };
    int16_t                 delta_pic_order_cnt_bottom { 0 };
    int16_t                 delta_pic_order_cnt[2] { 0 };
    uint32_t                redundant_pic_cnt { 0 };
    uint8_t                 direct_spatial_mv_pred_flag { 0 };
    uint8_t                 num_ref_idx_active_override_flag { 0 };
    uint32_t                num_ref_idx_l0_active_minus1 { 0 };
    uint32_t                num_ref_idx_l1_active_minus1 { 0 };
    uint32_t                cabac_init_idc { 0 };
    int32_t                 slice_qp_delta { 0 };
    uint8_t                 sp_for_switch_flag { 0 };
    int32_t                 slice_qs_delta { 0 };
    uint32_t                disable_deblocking_filter_idc { 0 };
    int32_t                 slice_alpha_c0_offset_div2 { 0 };
    int32_t                 slice_beta_offset_div2 { 0 };
    uint32_t                slice_group_change_cycle { 0 };

    RefPicListModParam_dt   ref_pic_list_modification;
    PredWeightTableParam_dt pred_weight_table;
    DecRefPicMarkParam_dt   dec_ref_pic_marking;

} SliceHeadParam_dt;

}; // namespace Common
