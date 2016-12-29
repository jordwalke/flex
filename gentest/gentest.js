/**
 * Copyright (c) 2014-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

// In the fixed point encoding of layout/style values, each field is measured in hundredths of pixels.
let unitsPerPixel = 100;
let zerosPerPixel = 2;
let intUnitsFromPixels = (v) => v != null ? '' + (v * unitsPerPixel) : '0';
let floatUnitsFromPixels = (v) => v != null ? '' + (v) + '.0' : '0.0';

let testDataToIntEncoding = (v) => v != null ? '' + (v) : '0';
let testDataToFloatEncoding = (v) => {
  let divided = '' + (v / unitsPerPixel);
  if (divided.indexOf('.') !== -1) {
    return divided;
  } else {
    return divided + '.0';
  }
};

/**
 * Test style data is in a JSON form that resembles the Reason records, but
 * string form where necessary (such as 'FlexGrow' instead of FlexGrow).  Test
 * data should be in the form of fixed integer encoding. We will convert it to
 * floating point as part of the test generation process if needed.
 */
window.testData =
  {"flex_grow_within_max_width":{"style":{"width":20000,"height":10000},"children":{"child0":{"style":{"flexDirection":"Row","maxWidth":10000},"children":{"child0":{"style":{"height":2000,"flexGrow":"1"}}}}}},"flex_grow_within_constrained_max_width":{"style":{"width":20000,"height":10000},"children":{"child0":{"style":{"flexDirection":"Row","maxWidth":30000},"children":{"child0":{"style":{"height":2000,"flexGrow":"1"}}}}}},"justify_content_overflow_min_max":{"style":{"minHeight":10000,"maxHeight":11000,"justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"width":5000,"height":5000}},"child1":{"style":{"width":5000,"height":5000}},"child2":{"style":{"width":5000,"height":5000}}}},"justify_content_min_max":{"style":{"maxHeight":20000,"minHeight":10000,"width":10000,"justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"width":6000,"height":6000}}}},"align_items_min_max":{"style":{"maxWidth":20000,"minWidth":10000,"height":10000,"alignItems":"AlignCenter"},"children":{"child0":{"style":{"width":6000,"height":6000}}}},"align_items_stretch":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"height":1000}}}},"align_items_center":{"style":{"width":10000,"height":10000,"alignItems":"AlignCenter"},"children":{"child0":{"style":{"height":1000,"width":1000}}}},"align_items_flex_start":{"style":{"width":10000,"height":10000,"alignItems":"AlignFlexStart"},"children":{"child0":{"style":{"height":1000,"width":1000}}}},"align_items_flex_end":{"style":{"width":10000,"height":10000,"alignItems":"AlignFlexEnd"},"children":{"child0":{"style":{"height":1000,"width":1000}}}},"align_self_center":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"height":1000,"width":1000,"alignSelf":"AlignCenter"}}}},"align_self_flex_end":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"height":1000,"width":1000,"alignSelf":"AlignFlexEnd"}}}},"align_self_flex_start":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"height":1000,"width":1000,"alignSelf":"AlignFlexStart"}}}},"align_self_flex_end_override_flex_start":{"style":{"width":10000,"height":10000,"alignItems":"AlignFlexStart"},"children":{"child0":{"style":{"height":1000,"width":1000,"alignSelf":"AlignFlexEnd"}}}},"border_no_size":{"style":{"border":1000}},"border_container_match_child":{"style":{"border":1000},"children":{"child0":{"style":{"width":1000,"height":1000}}}},"border_stretch_child":{"style":{"width":10000,"height":10000,"border":1000},"children":{"child0":{"style":{"height":1000}}}},"border_center_child":{"style":{"width":10000,"height":10000,"borderTop":1000,"borderBottom":2000,"alignItems":"AlignCenter","justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"height":1000,"width":1000}}}},"max_width":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"height":1000,"maxWidth":5000}}}},"max_height":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"width":1000,"maxHeight":5000}}}},"padding_no_size":{"style":{"padding":1000}},"padding_container_match_child":{"style":{"padding":1000},"children":{"child0":{"style":{"width":1000,"height":1000}}}},"padding_stretch_child":{"style":{"width":10000,"height":10000,"padding":1000},"children":{"child0":{"style":{"height":1000}}}},"padding_center_child":{"style":{"width":10000,"height":10000,"paddingTop":1000,"paddingBottom":2000,"alignItems":"AlignCenter","justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"height":1000,"width":1000}}}},"absolute_layout_width_height_start_top":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"width":1000,"height":1000,"positionType":"Absolute","top":1000}}}},"absolute_layout_width_height_end_bottom":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"width":1000,"height":1000,"positionType":"Absolute","bottom":1000}}}},"absolute_layout_start_top_end_bottom":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"positionType":"Absolute","top":1000,"bottom":1000}}}},"absolute_layout_width_height_start_top_end_bottom":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"width":1000,"height":1000,"positionType":"Absolute","top":1000,"bottom":1000}}}},"do_not_clamp_height_of_absolute_node_to_height_of_its_overflow_hidden_parent":{"style":{"height":5000,"width":5000,"overflow":"Hidden","flexDirection":"Row"},"children":{"child0":{"style":{"positionType":"Absolute","top":0},"children":{"child0":{"style":{"width":10000,"height":10000}}}}}},"flex_direction_column_no_height":{"style":{"width":10000},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"flex_direction_row_no_width":{"style":{"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"flex_direction_column":{"style":{"height":10000,"width":10000},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"flex_direction_row":{"style":{"height":10000,"width":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"flex_direction_column_reverse":{"style":{"height":10000,"width":10000,"flexDirection":"ColumnReverse"},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"flex_direction_row_reverse":{"style":{"height":10000,"width":10000,"flexDirection":"RowReverse"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"wrap_column":{"style":{"height":10000,"width":6000,"flexWrap":"CssWrap"},"children":{"child0":{"style":{"height":3000,"width":3000}},"child1":{"style":{"height":3000,"width":3000}},"child2":{"style":{"height":3000,"width":3000}},"child3":{"style":{"height":3000,"width":3000}}}},"wrap_row":{"style":{"width":10000,"flexDirection":"Row","flexWrap":"CssWrap"},"children":{"child0":{"style":{"height":3000,"width":3000}},"child1":{"style":{"height":3000,"width":3000}},"child2":{"style":{"height":3000,"width":3000}},"child3":{"style":{"height":3000,"width":3000}}}},"wrap_row_align_items_flex_end":{"style":{"width":10000,"flexDirection":"Row","flexWrap":"CssWrap","alignItems":"AlignFlexEnd"},"children":{"child0":{"style":{"height":1000,"width":3000}},"child1":{"style":{"height":2000,"width":3000}},"child2":{"style":{"height":3000,"width":3000}},"child3":{"style":{"height":3000,"width":3000}}}},"wrap_row_align_items_center":{"style":{"width":10000,"flexDirection":"Row","flexWrap":"CssWrap","alignItems":"AlignCenter"},"children":{"child0":{"style":{"height":1000,"width":3000}},"child1":{"style":{"height":2000,"width":3000}},"child2":{"style":{"height":3000,"width":3000}},"child3":{"style":{"height":3000,"width":3000}}}},"margin_start":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"width":1000}}}},"margin_end":{"style":{"width":10000,"height":10000,"flexDirection":"Row","justifyContent":"JustifyFlexEnd"},"children":{"child0":{"style":{"width":1000}}}},"margin_left":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"width":1000,"marginLeft":1000}}}},"margin_top":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"height":1000,"marginTop":1000}}}},"margin_right":{"style":{"width":10000,"height":10000,"flexDirection":"Row","justifyContent":"JustifyFlexEnd"},"children":{"child0":{"style":{"width":1000,"marginRight":1000}}}},"margin_bottom":{"style":{"width":10000,"height":10000,"justifyContent":"JustifyFlexEnd"},"children":{"child0":{"style":{"height":1000,"marginBottom":1000}}}},"align_content_flex_start":{"style":{"width":10000,"height":10000,"flexWrap":"CssWrap","flexDirection":"Column","alignContent":"AlignFlexStart"},"children":{"child0":{"style":{"width":5000,"height":1000}},"child1":{"style":{"width":5000,"height":1000}},"child2":{"style":{"width":5000,"height":1000}},"child3":{"style":{"width":5000,"height":1000}},"child4":{"style":{"width":5000,"height":1000}}}},"align_content_flex_end":{"style":{"width":10000,"height":10000,"flexWrap":"CssWrap","flexDirection":"Column","alignContent":"AlignFlexEnd"},"children":{"child0":{"style":{"width":5000,"height":1000}},"child1":{"style":{"width":5000,"height":1000}},"child2":{"style":{"width":5000,"height":1000}},"child3":{"style":{"width":5000,"height":1000}},"child4":{"style":{"width":5000,"height":1000}}}},"align_content_center":{"style":{"width":10000,"height":10000,"flexWrap":"CssWrap","flexDirection":"Column","alignContent":"AlignCenter"},"children":{"child0":{"style":{"width":5000,"height":1000}},"child1":{"style":{"width":5000,"height":1000}},"child2":{"style":{"width":5000,"height":1000}},"child3":{"style":{"width":5000,"height":1000}},"child4":{"style":{"width":5000,"height":1000}}}},"align_content_stretch":{"style":{"width":10000,"height":10000,"flexWrap":"CssWrap","flexDirection":"Column","alignContent":"AlignStretch"},"children":{"child0":{"style":{"width":5000}},"child1":{"style":{"width":5000}},"child2":{"style":{"width":5000}},"child3":{"style":{"width":5000}},"child4":{"style":{"width":5000}}}},"justify_content_row_flex_start":{"style":{"width":10200,"height":10200,"flexDirection":"Row","justifyContent":"JustifyFlexStart"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"justify_content_row_flex_end":{"style":{"width":10200,"height":10200,"flexDirection":"Row","justifyContent":"JustifyFlexEnd"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"justify_content_row_center":{"style":{"width":10200,"height":10200,"flexDirection":"Row","justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"justify_content_row_space_between":{"style":{"width":10200,"height":10200,"flexDirection":"Row","justifyContent":"JustifySpaceBetween"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"justify_content_row_space_around":{"style":{"width":10200,"height":10200,"flexDirection":"Row","justifyContent":"JustifySpaceAround"},"children":{"child0":{"style":{"width":1000}},"child1":{"style":{"width":1000}},"child2":{"style":{"width":1000}}}},"justify_content_column_flex_start":{"style":{"width":10200,"height":10200,"justifyContent":"JustifyFlexStart"},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"justify_content_column_flex_end":{"style":{"width":10200,"height":10200,"justifyContent":"JustifyFlexEnd"},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"justify_content_column_center":{"style":{"width":10200,"height":10200,"justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"justify_content_column_space_between":{"style":{"width":10200,"height":10200,"justifyContent":"JustifySpaceBetween"},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"justify_content_column_space_around":{"style":{"width":10200,"height":10200,"justifyContent":"JustifySpaceAround"},"children":{"child0":{"style":{"height":1000}},"child1":{"style":{"height":1000}},"child2":{"style":{"height":1000}}}},"border_flex_child":{"style":{"width":10000,"height":10000,"border":1000},"children":{"child0":{"style":{"width":1000,"flexGrow":"1"}}}},"min_height":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"flexGrow":"1","minHeight":6000}},"child1":{"style":{"flexGrow":"1"}}}},"min_width":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"flexGrow":"1","minWidth":6000}},"child1":{"style":{"flexGrow":"1"}}}},"padding_flex_child":{"style":{"width":10000,"height":10000,"padding":1000},"children":{"child0":{"style":{"width":1000,"flexGrow":"1"}}}},"margin_and_flex_row":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"flexGrow":"1"}}}},"margin_and_flex_column":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"marginTop":1000,"marginBottom":1000,"flexGrow":"1"}}}},"margin_and_stretch_row":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"marginTop":1000,"marginBottom":1000,"flexGrow":"1"}}}},"margin_and_stretch_column":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"flexGrow":"1"}}}},"margin_with_sibling_row":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"flexGrow":"1"}},"child1":{"style":{"flexGrow":"1"}}}},"margin_with_sibling_column":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"marginBottom":1000,"flexGrow":"1"}},"child1":{"style":{"flexGrow":"1"}}}},"flex_basis_flex_grow_column":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"flexBasis":"50px","flexGrow":"1"}},"child1":{"style":{"flexGrow":"1"}}}},"flex_basis_flex_grow_row":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"flexBasis":"50px","flexGrow":"1"}},"child1":{"style":{"flexGrow":"1"}}}},"flex_basis_flex_shrink_column":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"flexBasis":"100px","flexShrink":"1"}},"child1":{"style":{"flexBasis":"50px"}}}},"flex_basis_flex_shrink_row":{"style":{"width":10000,"height":10000,"flexDirection":"Row"},"children":{"child0":{"style":{"flexBasis":"100px","flexShrink":"1"}},"child1":{"style":{"flexBasis":"50px"}}}},"flex_basis_flex_grow_undefined_main":{"style":{"width":10000},"children":{"child0":{"style":{"flexBasis":"100px","flexGrow":"1"}},"child1":{"style":{"flexBasis":"50px"}}}},"jwalke_border_width_only_start":{"style":{"width":10000,"height":10000,"borderTop":1000,"borderBottom":2000,"alignItems":"AlignCenter","justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"height":1000,"width":1000}}}},"jwalke_border_width_only_end":{"style":{"width":10000,"height":10000,"borderTop":1000,"borderBottom":2000,"alignItems":"AlignCenter","justifyContent":"JustifyCenter"},"children":{"child0":{"style":{"height":1000,"width":1000}}}},"start_overrides_margin":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"marginLeft":2000,"marginRight":2000,"flexGrow":"1"}}}},"end_overrides_margin":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"marginLeft":2000,"marginRight":2000,"flexGrow":"1"}}}},"start_overrides_padding":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"paddingLeft":2000,"paddingRight":2000,"flexGrow":"1"}}}},"end_overrides_padding":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"paddingLeft":2000,"paddingRight":2000,"flexGrow":"1"}}}},"start_overrides_border":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"borderLeft":2000,"borderRight":2000,"flexGrow":"1"}}}},"end_overrides_border":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"borderLeft":2000,"borderRight":2000,"flexGrow":"1"}}}},"start_overrides":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"positionType":"Absolute","left":2000,"right":2000,"flexGrow":"1"}}}},"end_overrides":{"style":{"width":10000,"height":10000},"children":{"child0":{"style":{"positionType":"Absolute","left":2000,"right":2000,"flexGrow":"1"}}}}};

/**
 * Used to render test data to markup for the purpose of measuring layout.
 */
var styleTestDataToMarkupMap = {
  'positionType': (key, val, isLtr) => ({key: 'position', val: positionTypeMarkupValue(val)}),
  'direction': (key, val, isLtr) => ({key: 'direction', val: directionMarkupValue(val)}),
  'flexDirection': (key, val, isLtr) => ({key: 'flex-direction', val: flexDirectionMarkupValue(val)}),
  'justifyContent': (key, val, isLtr) => ({key: 'justify-content', val: justifyMarkupValue(val)}),
  'alignContent': (key, val, isLtr) => ({key: 'align-content', val: alignMarkupValue(val)}),
  'alignItems': (key, val, isLtr) => ({key: 'align-items', val: alignMarkupValue(val)}),
  'alignSelf': (key, val, isLtr) => ({key: 'align-self', val: alignMarkupValue(val)}),
  'overflow': (key, val, isLtr) => ({key: 'overflow', val: overflowMarkupValue(val)}),
  'flexWrap': (key, val, isLtr) => ({key: 'flex-wrap', val: wrapMarkupValue(val)}),
  'flexGrow': (key, val, isLtr) => ({key: 'flex-grow', val: val}),
  'flexShrink': (key, val, isLtr) => ({key: 'flex-shrink', val: val}),
  'flexBasis': (key, val, isLtr) =>  ({key: 'flex-basis', val: pixelMarkupValue(val)}),
  // Position.
  // Not sure `position` is even a real css property (It represents "all").
  'position': (key, val, isLtr) => ({key: 'position', val: pixelMarkupValue(val)}),
  'top': (key, val, isLtr) => ({key: 'top', val: pixelMarkupValue(val)}),
  'bottom': (key, val, isLtr) => ({key: 'bottom', val: pixelMarkupValue(val)}),
  'left': (key, val, isLtr) => ({key: 'left', val: pixelMarkupValue(val)}),
  'right': (key, val, isLtr) => ({key: 'right', val: pixelMarkupValue(val)}),
  'start': (key, val, isLtr) => ({key: startFor(isLtr), val: pixelMarkupValue(val)}),
  'endd': (key, val, isLtr) => ({key: endFor(isLtr), val: pixelMarkupValue(val)}),
  'vertical': (key, val, isLtr) => ({key: 'vertical', val: pixelMarkupValue(val)}),
  'horizontal': (key, val, isLtr) => ({key: 'horizontal', val: pixelMarkupValue(val)}),

  // Margin.
  'margin': (key, val, isLtr) => ({key: 'margin', val: pixelMarkupValue(val)}),
  'marginTop': (key, val, isLtr) => ({key: 'margin-top', val: pixelMarkupValue(val)}),
  'marginBottom': (key, val, isLtr) => ({key: 'margin-bottom', val: pixelMarkupValue(val)}),
  'marginLeft': (key, val, isLtr) => ({key: 'margin-left', val: pixelMarkupValue(val)}),
  'marginRight': (key, val, isLtr) => ({key: 'margin-right', val: pixelMarkupValue(val)}),
  'marginStart': (key, val, isLtr) => ({key: 'margin-' + startFor(isLtr), val: pixelMarkupValue(val)}),
  'marginEnd': (key, val, isLtr) => ({key: 'margin-' + endFor(isLtr), val: pixelMarkupValue(val)}),
  'marginVertical': (key, val, isLtr) => ({key: 'margin-vertical', val: pixelMarkupValue(val)}),
  'marginHorizontal': (key, val, isLtr) => ({key: 'margin-horizontal', val: pixelMarkupValue(val)}),

  // Padding.
  'padding': (key, val, isLtr) => ({key: 'padding', val: pixelMarkupValue(val)}),
  'paddingTop': (key, val, isLtr) => ({key: 'padding-top', val: pixelMarkupValue(val)}),
  'paddingBottom': (key, val, isLtr) => ({key: 'padding-bottom', val: pixelMarkupValue(val)}),
  'paddingLeft': (key, val, isLtr) => ({key: 'padding-left', val: pixelMarkupValue(val)}),
  'paddingRight': (key, val, isLtr) => ({key: 'padding-right', val: pixelMarkupValue(val)}),
  'paddingStart': (key, val, isLtr) => ({key: 'padding-' + startFor(isLtr), val: pixelMarkupValue(val)}),
  'paddingEnd': (key, val, isLtr) => ({key: 'padding-' + endFor(isLtr), val: pixelMarkupValue(val)}),
  'paddingHorizontal': (key, val, isLtr) => ({key: 'padding-horizontal', val: pixelMarkupValue(val)}),
  'paddingVertical': (key, val, isLtr) => ({key: 'padding-vertical', val: pixelMarkupValue(val)}),

  // Border.
  'border': (key, val, isLtr) => ({key: 'border', val: pixelMarkupValue(val)}),
  'borderTop': (key, val, isLtr) => ({key: 'border-top-width', val: pixelMarkupValue(val)}),
  'borderBottom': (key, val, isLtr) => ({key: 'border-bottom-width', val: pixelMarkupValue(val)}),
  'borderLeft': (key, val, isLtr) => ({key: 'border-left-width', val: pixelMarkupValue(val)}),
  'borderRight': (key, val, isLtr) => ({key: 'border-right-width', val: pixelMarkupValue(val)}),
  'borderStart': (key, val, isLtr) => ({key: 'border-' + startFor(isLtr) + '-width', val: pixelMarkupValue(val)}),
  'borderEnd': (key, val, isLtr) => ({key: 'border-' + endFor(isLtr) + '-width', val: pixelMarkupValue(val)}),
  'borderHorizontal': (key, val, isLtr) => ({key: 'border-horizontal-width', val: pixelMarkupValue(val)}),
  'borderVertical': (key, val, isLtr) => ({key: 'border-vertical-width', val: pixelMarkupValue(val)}),

  // Dimensions.
  'width': (key, val, isLtr) => ({key: 'width', val: pixelMarkupValue(val)}),
  'height': (key, val, isLtr) => ({key: 'height', val: pixelMarkupValue(val)}),
  'minWidth': (key, val, isLtr) => ({key: 'min-width', val: pixelMarkupValue(val)}),
  'maxWidth': (key, val, isLtr) => ({key: 'max-width', val: pixelMarkupValue(val)}),
  'minHeight': (key, val, isLtr) => ({key: 'min-height', val: pixelMarkupValue(val)}),
  'maxHeight': (key, val, isLtr) => ({key: 'max-height', val: pixelMarkupValue(val)}),
};

let capitalize = (s) => {
  return s.charAt(0).toUppercase() + s.substr(1);
};

/**
 * This converts the other direction - from markup, to style test data.  This
 * is only even used to convert html snippets into test data, which is then
 * converted *back* into HTML that is actually measured.  This is only needed
 * when adding new test cases.
 */
var markupToStyleTestDatapMap = {
  'position': (key, val, useFloats) => ({key: 'positionType', val: positionTypeValue(val)}),
  'direction': (key, val, useFloats) => ({key: 'direction', val: directionValue(val)}),
  'flex-direction': (key, val, useFloats) => ({key: 'flexDirection', val: flexDirectionValue(val)}),
  'justify-content': (key, val, useFloats) => ({key: 'justifyContent', val: justifyValue(val)}),
  'align-content': (key, val, useFloats) => ({key: 'alignContent', val: alignValue(val)}),
  'align-items': (key, val, useFloats) => ({key: 'alignItems', val: alignValue(val)}),
  'align-self': (key, val, useFloats) => ({key: 'alignSelf', val: alignValue(val)}),
  'overflow': (key, val, useFloats) => ({key: 'overflow', val: overflowValue(val)}),
  'flex-wrap': (key, val, useFloats) => ({key: 'flexWrap', val: wrapValue(val)}),
  'flex-grow': (key, val, useFloats) => ({key: 'flexGrow', val: val}),
  'flex-shrink': (key, val, useFloats) => ({key: 'flexShrink', val: val}),
  'flex-basis': (key, val, useFloats) =>  ({key: 'flexBasis', val: markupPixelValueToIntegerEncoding(val)}),
  // Position.
  // Not sure `position` is even a real css property (It represents "all").
  //'position': (key, val, useFloats) => ({key: 'position', val: markupPixelValueToIntegerEncoding(val)}),
  'top': (key, val, useFloats) => ({key: 'top', val: markupPixelValueToIntegerEncoding(val)}),
  'bottom': (key, val, useFloats) => ({key: 'bottom', val: markupPixelValueToIntegerEncoding(val)}),
  'left': (key, val, useFloats) => ({key: 'left', val: markupPixelValueToIntegerEncoding(val)}),
  'right': (key, val, useFloats) => ({key: 'right', val: markupPixelValueToIntegerEncoding(val)}),
  'start': (key, val, useFloats) => ({key: 'start', val: markupPixelValueToIntegerEncoding(val)}),
  'end': (key, val, useFloats) => ({key: 'endd', val: markupPixelValueToIntegerEncoding(val)}),
  'vertical': (key, val, useFloats) => ({key: 'vertical', val: markupPixelValueToIntegerEncoding(val)}),
  'horizontal': (key, val, useFloats) => ({key: 'horizontal', val: markupPixelValueToIntegerEncoding(val)}),

  // Margin.
  'margin': (key, val, useFloats) => ({key: 'margin', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-top': (key, val, useFloats) => ({key: 'marginTop', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-bottom': (key, val, useFloats) => ({key: 'marginBottom', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-left': (key, val, useFloats) => ({key: 'marginLeft', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-right': (key, val, useFloats) => ({key: 'marginRight', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-start': (key, val, useFloats) => ({key: 'marginStart', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-end': (key, val, useFloats) => ({key: 'marginEnd', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-vertical': (key, val, useFloats) => ({key: 'marginVertical', val: markupPixelValueToIntegerEncoding(val)}),
  'margin-horizontal': (key, val, useFloats) => ({key: 'marginHorizontal', val: markupPixelValueToIntegerEncoding(val)}),

  // Padding.
  'padding': (key, val, useFloats) => ({key: 'padding', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-top': (key, val, useFloats) => ({key: 'paddingTop', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-bottom': (key, val, useFloats) => ({key: 'paddingBottom', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-left': (key, val, useFloats) => ({key: 'paddingLeft', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-right': (key, val, useFloats) => ({key: 'paddingRight', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-start': (key, val, useFloats) => ({key: 'paddingStart', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-end': (key, val, useFloats) => ({key: 'paddingEnd', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-horizontal': (key, val, useFloats) => ({key: 'paddingHorizontal', val: markupPixelValueToIntegerEncoding(val)}),
  'padding-vertical': (key, val, useFloats) => ({key: 'paddingVertical', val: markupPixelValueToIntegerEncoding(val)}),

  // Border.
  'border-width': (key, val, useFloats) => ({key: 'border', val: markupPixelValueToIntegerEncoding(val)}),
  'border-top-width': (key, val, useFloats) => ({key: 'borderTop', val: markupPixelValueToIntegerEncoding(val)}),
  'border-bottom-width': (key, val, useFloats) => ({key: 'borderBottom', val: markupPixelValueToIntegerEncoding(val)}),
  'border-left-width': (key, val, useFloats) => ({key: 'borderLeft', val: markupPixelValueToIntegerEncoding(val)}),
  'border-right-width': (key, val, useFloats) => ({key: 'borderRight', val: markupPixelValueToIntegerEncoding(val)}),
  'border-start-width': (key, val, useFloats) => ({key: 'borderStart', val: markupPixelValueToIntegerEncoding(val)}),
  'border-end-width': (key, val, useFloats) => ({key: 'borderEnd', val: markupPixelValueToIntegerEncoding(val)}),
  'border-horizontal-width': (key, val, useFloats) => ({key: 'borderHorizontal', val: markupPixelValueToIntegerEncoding(val)}),
  'border-vertical-width': (key, val, useFloats) => ({key: 'borderVertical', val: markupPixelValueToIntegerEncoding(val)}),

  // Dimensions.
  'width': (key, val, useFloats) => ({key: 'width', val: markupPixelValueToIntegerEncoding(val)}),
  'height': (key, val, useFloats) => ({key: 'height', val: markupPixelValueToIntegerEncoding(val)}),
  'min-width': (key, val, useFloats) => ({key: 'minWidth', val: markupPixelValueToIntegerEncoding(val)}),
  'max-width': (key, val, useFloats) => ({key: 'maxWidth', val: markupPixelValueToIntegerEncoding(val)}),
  'min-height': (key, val, useFloats) => ({key: 'minHeight', val: markupPixelValueToIntegerEncoding(val)}),
  'max-height': (key, val, useFloats) => ({key: 'maxHeight', val: markupPixelValueToIntegerEncoding(val)}),
};

/**
 * While we don't yet support some expanded properties (such as
 * border-vertical-width = border-left=width + border-right-width), we will map
 * them to all the individual properties for now.
 */
var expandedEquivalentStyleKeys = {
  'position': ['top', 'bottom', 'left', 'right'],
  'vertical': ['top', 'bottom'],
  'horizontal': ['left', 'right'],

  // Margin.
  'margin': ['marginTop', 'marginBottom', 'marginLeft', 'marginRight'],
  'margin-vertical': ['marginTop', 'marginBottom'],
  'margin-horizontal': ['marginLeft', 'marginRight'],

  // Padding.
  'padding': ['paddingTop', 'paddingBottom', 'paddingLeft', 'paddingRight'],
  'padding-vertical': ['paddingTop', 'paddingBottom'],
  'padding-horizontal': ['paddingLeft', 'paddingRight'],

  // Border.
  'border': ['borderTop', 'borderBottom', 'borderLeft', 'borderRight'],
  'border-vertical': ['borderTop', 'borderBottom'],
  'border-horizontal': ['borderLeft', 'borderRight'],
};

var computeStyleStringForTestDataItem = (testDataItem, isLtr) => {
  var styleString = 'style="';
  for (var styleKey in testDataItem.style) {
    // Skip over start/endd because we want to make sure they appear last.
    if (styleKey !== 'start' && styleKey !== 'endd') {
      if (!styleTestDataToMarkupMap[styleKey]) {
        throw new Error("Encountered unsupported style key " + styleKey + ". Styles should be in terms of ReLayout styles.");
      }
      var styleKeyVal = styleTestDataToMarkupMap[styleKey](styleKey, testDataItem.style[styleKey], isLtr);
      styleString += styleKeyVal.key + ':' + styleKeyVal.val + '; ';
    }
  }
  if (testDataItem.style['start']) {
    let styleKeyVal = styleTestDataToMarkupMap['start']('start', testDataItem.style['start'], isLtr);
    styleString += styleKeyVal.key + ':' + styleKeyVal.val + '; ';
  }
  if (testDataItem.style['endd']) {
    let styleKeyVal = styleTestDataToMarkupMap['endd']('endd', testDataItem.style['endd'], isLtr);
    styleString += styleKeyVal.key + ':' + styleKeyVal.val + '; ';
  }
  styleString += '"';
  return styleString;
};

let legacyDomToJson = function(container) {
  if (!container) {
    return null;
  }
  let ret = {};
  var styleAttr = container.getAttribute('style');
  var styleItms = !styleAttr ? [] : styleAttr.split(';').filter(s => (s.trim() !== '')).map(seg => {
    let kv = seg.split(':');
    let key = kv[0].trim();
    let val = kv[1].trim();
    if (!markupToStyleTestDatapMap[key]) {
      return null;
    }
    let convertedKv = markupToStyleTestDatapMap[key](key, val, false);
    return {key: convertedKv.key, val: convertedKv.val};
  }).filter(x => !!x);
  let style = {};
  for (var i = 0; i < styleItms.length; i++) {
    let styleItm = styleItms[i];
    let styleItmKey = styleItm.key;
    if (expandedEquivalentStyleKeys[styleItmKey]) {
      let expandedKeys = expandedEquivalentStyleKeys[styleItmKey]
      for (var i = 0; i < expandedKeys.length; i++) {
        let expandedKey = expandedKeys[i];
        style[expandedKey] = styleItm.val;
      }
    } else {
      style[styleItm.key] = styleItm.val;
    }
  }
  ret.style = style;
  if (container.children.length) {
    let children = {};
    for (var i = 0; i < container.children.length; i++) {
      var domChild = container.children[i];
      children[domChild.id || 'child' + i] = legacyDomToJson(domChild);
    }
    ret.children = children;
  }
  return ret;
};

window.legacyMarkupToJson = function(markup) {
  let container = document.createElement('div');
  container.innerHTML = markup;
  return legacyDomToJson(container, false).children;
};

let forEachTestData = (testData, computedKeyPathSoFar, cb) => {
  for (var testKey in testData) {
    let totalKeyPath = computedKeyPathSoFar + '_' + testKey;
    let testDataItem = testData[testKey];
    cb(testData[testKey], testKey, totalKeyPath);
  }
};


var startFor = (isLtr) => isLtr ? 'left' : 'right';
var endFor = (isLtr) => isLtr ? 'right' : 'left';
function overflowValue(value) {
  switch (value) {
    case 'visible': return 'Visible';
    case 'hidden': return 'Hidden';
  }
}

function wrapValue(value) {
  switch (value) {
    case 'wrap': return 'CssWrap';
    case 'nowrap': return 'CssNoWrap';
  }
}

function flexDirectionValue(value) {
  switch (value) {
    case 'row': return 'Row';
    case 'row-reverse': return 'RowReverse';
    case 'column': return 'Column';
    case 'column-reverse': return 'ColumnReverse';
  }
}

function justifyValue(value) {
  switch (value) {
    case 'center': return 'JustifyCenter';
    case 'space-around': return 'JustifySpaceAround';
    case 'space-between': return 'JustifySpaceBetween';
    case 'flex-start': return 'JustifyFlexStart';
    case 'flex-end': return 'JustifyFlexEnd';
  }
}

function positionTypeValue(value) {
  switch (value) {
    case 'absolute': return 'Absolute';
    default: return 'Relative'
  }
}

function directionValue(value) {
  switch (value) {
    case 'ltr': return 'Ltr';
    case 'rtl': return 'Rtl';
    case 'inherit': return 'Inherit';
  }
}

function alignValue(value) {
  switch (value) {
    case 'auto': return 'AlignAuto';
    case 'center': return 'AlignCenter';
    case 'stretch': return 'AlignStretch';
    case 'flex-start': return 'AlignFlexStart';
    case 'flex-end': return 'AlignFlexEnd';
  }
}

function markupPixelValueToIntegerEncoding(value) {
  switch (value) {
    case 'auto': return 'cssUndefined';
    case 'undefined': return 'cssUndefined';
    default:
      let ret = Math.trunc(((+(value.replace('px', ''))) * unitsPerPixel));
      return ret;
  }
}

/**
 * Used to turn test data into the actual test cases.
 *
 */
var styleTestDataToEncodingMap = {
  /**
   * @param {bool} useFloats Whether or not to encode data in floats vs. fixed
   * point.
   */
  'flexGrow': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'flexShrink': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'flexBasis': (val, useFloats) =>  ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  // Position.
  // Not sure `position` is even a real css property (It represents "all").
  'position': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'top': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'bottom': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'left': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'right': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'start': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'endd': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'vertical': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'horizontal': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),

  // Margin.
  'margin': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginTop': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginBottom': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginLeft': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginRight': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginStart': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginEnd': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginVertical': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'marginHorizontal': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),

  // Padding.
  'padding': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingTop': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingBottom': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingLeft': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingRight': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingStart': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingEnd': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingHorizontal': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'paddingVertical': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),

  // Border.
  'border': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderTop': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderBottom': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderLeft': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderRight': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderStart': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderEnd': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderHorizontal': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'borderVertical': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),

  // Dimensions.
  'width': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'height': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'minWidth': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'maxWidth': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'minHeight': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val)),
  'maxHeight': (val, useFloats) => ((useFloats ? testDataToFloatEncoding : testDataToIntEncoding)(val))
};


window.generateMarkup = (testData, computedKeyPathSoFar, isLtr) => {
  var res = "";
  forEachTestData(testData, computedKeyPathSoFar, (testDataItem, testKey, totalKeyPath) => {
    var idString = 'id="' + totalKeyPath + '" ';
    var styleString = computeStyleStringForTestDataItem(testDataItem, isLtr);
    let opn = "<div " + idString + styleString + (testDataItem.children ? ">\n  " : "> </DIV>\n");
    let childText = window.generateMarkup(testDataItem.children, totalKeyPath, isLtr);
    let cls = testDataItem.children ? "\n</div>\n" : '';
    res += opn + childText + cls;
  });
  return res;
};

window.onload = function() {
  /**
   * The buttons have event handlers that look for these values.
   */
  document.body.children[0].innerHTML = (window.generateMarkup(window.testData, 'ltr', true));
  document.body.children[1].innerHTML = (window.generateMarkup(window.testData, 'rtl', false));
  window.fixedPointTest = window.printTest(window.testData, false, document.body.children[0], document.body.children[1]);
  window.floatingPointTest = window.printTest(window.testData, true, document.body.children[0], document.body.children[1]);
  document.getElementById('testDataViewer').innerHTML = JSON.stringify(window.testData);
}


window.printTest = function(testData, useFloats, LTRContainer, RTLContainer) {
  testNum = 0;
  var commentLines = [
    '/*',
    ' * vim: set ft=rust:',
    ' * vim: set ft=reason:',
    ' */',
    '/**',
    ' * Copyright (c) 2014-present, Facebook, Inc.',
    ' * All rights reserved.',
    ' *',
    ' * This source code is licensed under the BSD-style license found in the',
    ' * LICENSE file in the root directory of this source tree. An additional grant',
    ' * of patent rights can be found in the PATENTS file in the same directory.',
    ' */',
    '',
  ];

  commentLines.push('/**');
  commentLines.push(' * @Generated by gentest/gentest.sh with the following input');
  commentLines.push(' *');

  var indentation = 0;
  commentLines.push(JSON.stringify(testData));
  commentLines.push(' *');
  commentLines.push(' */');
  commentLines.push('');

  var ltrExpectedResults = getExpectedResults(testData, 'ltr');
  var rtlExpectedResults = getExpectedResults(testData, 'rtl');

  let testNameLines = [];
  let testLines = [];
  /**
   * Lines containing function definitions, one for each benchmark.
   */
  let benchmarkFunctionDefinitions = [];
  /**
   * List of core_bench benchmarks.
   */
  let reasonListOfCoreBenchmarks = [
    'let benchmarks = [];'
  ];
  /**
   * All the function bodies of the reasonListOfCoreBenchmarks functions in one
   * list so the entire set may be tested as a single unit.
   */
  let allBenchmarkLinesAsOne = [];

  forEachTestData(testData, '', (testDataItem, testKey, totalKeyPath) => {
    testNameLines.push('let ' + testKey + ' = "'+ testKey + '";');
    let setupTreeStr = '  ' + setupTestTree(useFloats, testKey, testDataItem, 'root').join('\n');
    let performLtrLine = '  Layout.layoutNode root cssUndefined cssUndefined Ltr;';
    let performRtlLine = '  Layout.layoutNode root cssUndefined cssUndefined Rtl;';
    testLines = testLines.concat([
      'it ' + testKey + ' (fun () => {',
      setupTreeStr,
      performLtrLine,
      '',
      '  ' + assertTestTree(useFloats, testDataItem, ltrExpectedResults[testKey], 'root', true).join('\n  '),
      '',
      performRtlLine,
      '',
      '  ' + assertTestTree(useFloats, testDataItem, rtlExpectedResults[testKey], 'root', true).join('\n  '),
      '});',
      ''
    ]);
    let thisBenchmarkFunctionName = 'bench_' + testKey;
    let thisBenchmarkFunction = [
      ' let ' + thisBenchmarkFunctionName + ' () => {',
      '  ' + setupTreeStr,
      '  ' + performLtrLine,
      '  ' + performRtlLine,
      '};'
    ];
    allBenchmarkLinesAsOne = allBenchmarkLinesAsOne.concat([
      '/* ' + testKey + ' */',
      setupTreeStr,
      performLtrLine,
      performRtlLine,
    ]);

    benchmarkFunctionDefinitions = benchmarkFunctionDefinitions.concat (thisBenchmarkFunction);
    reasonListOfCoreBenchmarks = reasonListOfCoreBenchmarks.concat([
      'let benchmarks = LayoutTestUtils.shouldRun ' + testKey + ' ?',
      '  [',
      '    Bench.Test.create name::' + testKey,
      '    ' + thisBenchmarkFunctionName + ',',
      '    ...benchmarks',
      '  ] :',
      '  benchmarks;'
    ]);
  });
  testLines.push('  LayoutTestUtils.displayOutcomes ();');
  reasonListOfCoreBenchmarks = reasonListOfCoreBenchmarks.map((line) => '  ' + line);
  testLines = testLines.map((line) => '  ' + line);
  let totalLines = [
    '{commentLines}',
    'let floatMult = ( *. );',
    'let floatDiv = (/.);',
    'let floatSub = (-.);',
    'let module Node = {type context = unit; let nullContext = ();};',
    'let module Encoding = ' + (useFloats ? 'FloatEncoding;' : 'FixedEncoding;'),
    'let module LayoutTestUtils = LayoutTestUtils.Create Node Encoding;',
    'let module Layout = Layout.Create Node Encoding;',
    'module LayoutSupport = Layout.LayoutSupport;',
    'open LayoutTestUtils;',
    'open Encoding;',
    '{testNameLines}',
    '/**',
    ' * Since Core_bench is such a huge dependency and it doesn\'t compile with byte,',
    ' * we include a fake shim of it that we enable by default. To use the far',
    ' * superior Core_bench.',
    ' *',
    ' * - Comment out `include FakeCore;` below.',
    ' * - Uncomment `open Core_bench.Std;`',
    ' * - Delete the two targets in package.json (byteTarget, jsTarget)',
    ' * - Run `npm run build`, then `npm run bench`',
    ' *',
    ' */',
    'let module FakeCore = {',
    '  let module Bench = {',
    '    let module Test = {',
    '      let create name::s itm => (s, itm);',
    '    };',
    '    let make_command listofCreatedBenchmarks => listofCreatedBenchmarks;',
    '  };',
    '  let module Core = {',
    '    let runCommand (name, func) => {',
    '      let numIterations = 10000;',
    '      let timesInMs = Array.make_float numIterations;',
    '      Gc.full_major ();',
    '      let startSeconds = Sys.time ();',
    '      for i in 0 to (numIterations - 1) {',
    '        let itemStartSeconds = Sys.time ();',
    '        func ();',
    '        let itemEndSeconds = Sys.time ();',
    '        timesInMs.(i) = floatSub itemEndSeconds itemStartSeconds',
    '      };',
    '      let endSeconds = Sys.time ();',
    '      print_string (',
    '        "Average ms for " ^',
    '        name ^',
    '        " " ^',
    '        string_of_float (',
    '          floatMult 1000.0 (floatDiv (floatSub endSeconds startSeconds) (float_of_int numIterations))',
    '        )',
    '      );',
    '      print_newline ();',
    '      Array.sort compare timesInMs;',
    '      print_string (',
    '        "Median ms for " ^ name ^ " " ^ string_of_float (floatMult 1000.0 timesInMs.(numIterations / 2)) ^ " (Not Valid For JS benchmarks)"',
    '      );',
    '      print_newline ()',
    '    };',
    '    let module Std = {',
    '      let module Command = {',
    '        let run listOftests => List.iter (fun command => runCommand command) listOftests;',
    '      };',
    '    };',
    '  };',
    '};',
    '',
    'include FakeCore;',
    '',
    '/* open Core_bench.Std; */',
    '',
    '',
    '',
    'if (LayoutTestUtils.runMode === Bench) {',
    '  if LayoutTestUtils.shouldBenchmarkAllAsOne {',
    '    Core.Std.Command.run (Bench.make_command [Bench.Test.create name::"all-benchmarks" (fun()=>{',
    '      {allBenchmarkLinesAsOne}',
    '    })]);',
    '  } else {',
    '    {benchmarkFunctionDefinitions}',
    '    {reasonListOfCoreBenchmarks}',
    '    Core.Std.Command.run (Bench.make_command benchmarks);',
    '  }',
    '} else {',
    '  {testLines}',
    '};'
  ].join('\n')
  .replace('{commentLines}', commentLines.join('\n'))
  .replace('{testNameLines}', testNameLines.join('\n'))
  .replace('{allBenchmarkLinesAsOne}', allBenchmarkLinesAsOne.join('\n        '))
  .replace('{benchmarkFunctionDefinitions}', benchmarkFunctionDefinitions.join('\n    '))
  .replace('{reasonListOfCoreBenchmarks}', reasonListOfCoreBenchmarks.join('\n    '))
  .replace('{testLines}', testLines.join('\n  '));
  return totalLines;
}

let testNum = 0;

let createLayoutExtensionNode = (useFloats, nodeName, top, left, width, height) => {
  let converter = useFloats ? floatUnitsFromPixels : intUnitsFromPixels;
  return (
    '{...' +
    nodeName +
    '.layout, top:layoutTop, left: layoutLeft, width: layoutWidth, height: layoutHeight}'
  ).replace('layoutTop', converter(top))
   .replace('layoutLeft', converter(left))
   .replace('layoutWidth', converter(width))
   .replace('layoutHeight', converter(height));
};

const createInequalityChecker = (useFloats, expectedResults, nodeName) => {
  let expectedLayout = expectedResults.expectedLayout;
  let converter = useFloats ? floatUnitsFromPixels : intUnitsFromPixels;
  return nodeName + '.layout.top != ' + converter(expectedLayout.top) + ' || ' +
    nodeName + '.layout.left != ' + converter(expectedLayout.left) + ' || ' +
    nodeName + '.layout.width != ' + converter(expectedLayout.width) + ' || ' +
    nodeName + '.layout.height != ' + converter(expectedLayout.height);
};

/**
 * Should render container validation iff
 *   parentName is null || hasChildren (because it's nice to see the node
 *   validated again as a container this time).
 */
function assertTestTreePrint(useFloats, expectedResults, nodeBindingName, isRootmost) {
  let shouldValidate = isRootmost || expectedResults.children != null;
  if (!shouldValidate) {
    return [].join('');
  } else {
    let childItems = [];
    forEachTestData(expectedResults.children, nodeBindingName, (expectedItem, testKey, totalKeyPath) => {
      let expectedLayout = expectedItem.expectedLayout;
      childItems.push(
        '  (' +
        createLayoutExtensionNode(useFloats, totalKeyPath, expectedLayout.top, expectedLayout.left, expectedLayout.width, expectedLayout.height) +
        ', ' +
        totalKeyPath +
        '.layout),'
      );
    });
    let childLines = ['['].concat(childItems).concat([']']);
    let expectedLayout = expectedResults.expectedLayout;
    let assertionPrintingLines =
      [
        'assertLayouts testNum (expectedContainerLayout, observedContainerLayout)'
          .replace('testNum', testNum++)
          .replace(
            'expectedContainerLayout',
            createLayoutExtensionNode(useFloats, nodeBindingName, expectedLayout.top, expectedLayout.left, expectedLayout.width, expectedLayout.height)
          )
          .replace('observedContainerLayout', nodeBindingName + '.layout'),
      ].concat(childLines).concat([';']);

    let recursePrettyPrintLines = [];
    forEachTestData(expectedResults.children, nodeBindingName, (expectedItem, testKey, totalKeyPath) => {
      recursePrettyPrintLines.push('');
      recursePrettyPrintLines = recursePrettyPrintLines.concat(
        assertTestTreePrint(useFloats, expectedItem, totalKeyPath, false)
      );
    });
    let doThePrettyPrintingTestStr = assertionPrintingLines.concat(recursePrettyPrintLines);
    return doThePrettyPrintingTestStr.join('\n');
  }
};

function assertTestTreeFast(useFloats, testData, expectedResults, nodeBindingName) {
  let ret = [
    createInequalityChecker(useFloats, expectedResults, nodeBindingName)
  ];
  forEachTestData(testData.children, nodeBindingName, (testDataItem, testKey, totalKeyPath) => {
    ret.push(assertTestTreeFast(useFloats, testDataItem, expectedResults.children[testKey], totalKeyPath));
  });
  return ret.join(' ||\n');
};

/**
 * @param {varType} testData Description
 * @param {object} expectedResults Tree that mirrors the testData.
 */
function assertTestTree(useFloats, testData, expectedResults, nodeBindingName, isRootmost) {
  let fastNumericCheckExprStr = assertTestTreeFast(useFloats, testData, expectedResults, nodeBindingName);
  let testAndPrettyPrint = [
    'if (' + fastNumericCheckExprStr + ') {',
    '  ' + assertTestTreePrint(useFloats, expectedResults, nodeBindingName, isRootmost),
    '};'
  ];
  return testAndPrettyPrint;
};

function setupTestTree(useFloats, testName, testData, letBindingNodeName) {
  var lines = [];
  var styleLines = [];
  for (var reasonStyleKey in testData.style) {
    let reasonStyleValue = testData.style[reasonStyleKey];
    let value =
      styleTestDataToEncodingMap[reasonStyleKey] ?
        styleTestDataToEncodingMap[reasonStyleKey](reasonStyleValue, useFloats) :
        reasonStyleValue;
    styleLines.push(reasonStyleKey + ': ' + value);
  }
  if (styleLines.length > 0) {
    lines = lines.concat([
      'let ' + letBindingNodeName + '_style = {',
      '  ...LayoutSupport.defaultStyle,'
    ]);
    lines = lines.concat(styleLines.map((sl) => '  ' + sl + ','));
    lines.push('};');
  } else {
    lines = lines.concat([ 'let ' + letBindingNodeName + '_style = LayoutSupport.defaultStyle;']);
  }
  let childrenArray = [];
  forEachTestData(testData.children, letBindingNodeName, (testDataItem, testKey, totalKeyPath) => {
    lines.push('');
    lines = lines.concat(setupTestTree(useFloats, testName + ' (child)', testDataItem, totalKeyPath));
    childrenArray.push(totalKeyPath);
  });
  lines.push(
    'let ' + letBindingNodeName + ' = LayoutSupport.createNode ' +
      'withChildren::[|' + childrenArray.join(',') + '|] ' +
      'andStyle::' + letBindingNodeName + '_style ();'
  );
  return lines;
}

function overflowMarkupValue(value) {
  switch (value) {
    case 'Visible': return 'visible';
    case 'Hidden': return 'hidden';
  }
}

function wrapMarkupValue(value) {
  switch (value) {
    case 'CssWrap': return 'wrap';
    case 'CssNoWrap': return 'nowrap';
  }
}

function flexDirectionMarkupValue(value) {
  switch (value) {
    case 'Row': return 'row';
    case 'RowReverse': return 'row-reverse';
    case 'Column': return 'column';
    case 'ColumnReverse': return 'column-reverse';
  }
}


function justifyMarkupValue(value) {
  switch (value) {
    case 'JustifyCenter': return 'center';
    case 'JustifySpaceAround': return 'space-around';
    case 'JustifySpaceBetween': return 'space-between';
    case 'JustifyFlexStart': return 'flex-start';
    case 'JustifyFlexEnd': return 'flex-end';
  }
}

function positionTypeMarkupValue(value) {
  switch (value) {
    case 'Absolute': return 'absolute';
    case 'Relative': return 'relative';
    default: throw new Error("Unknown position value: " + value);
  }
}

function directionMarkupValue(value) {
  switch (value) {
    case 'Ltr': return 'ltr';
    case 'Rtl': return 'rtl';
    case 'Inherit': return 'inherit';
  }
}

function alignMarkupValue(value) {
  switch (value) {
    case 'AlignAuto': return 'auto';
    case 'AlignCenter': return 'center';
    case 'AlignStretch': return 'stretch';
    case 'AlignFlexStart': return 'flex-start';
    case 'AlignFlexEnd': return 'flex-end';
  }
}

function pixelMarkupValue(fixedEncodingValue) {
  switch (fixedEncodingValue) {
    case 'cssUndefined': return 'auto';
    default:
      return (fixedEncodingValue / unitsPerPixel) + 'px';
  }
}

/**
 * Returns a structure that mirrors the test data.
 */
function getExpectedResults(testData, computedKeyPathSoFar) {
  var ret = {};
  forEachTestData(testData, computedKeyPathSoFar, (testDataItem, testKey, totalKeyPath) => {
    let domNode = document.getElementById(totalKeyPath);
    ret[testKey] = {
      expectedLayout: {
        left: domNode.offsetLeft + domNode.parentNode.clientLeft,
        top: domNode.offsetTop + domNode.parentNode.clientTop,
        width: domNode.offsetWidth,
        height: domNode.offsetHeight,
      },
      children: testData[testKey].children ? getExpectedResults(testData[testKey].children, totalKeyPath) : null
    };
  });
  return ret;
}


/* Supported:

<div id="flex_grow_within_max_width" style="width: 200px; height: 100px;">
   <div style="flex-direction: row; max-width: 100px;">
     <div style=" flex-grow: 1; height: 20px;"></div>
   </div>
 </div>

 <div id="flex_grow_within_constrained_max_width" style="width: 200px; height: 100px;">
   <div style="flex-direction: row; max-width: 300px;">
     <div style=" flex-grow: 1; height: 20px;"></div>
   </div>
 </div>

 <div id="justify_content_overflow_min_max" style="justify-content: center; min-height: 100px; max-height: 110px; ">
   <div style="width: 50px; height: 50px;"></div>
   <div style="width: 50px; height: 50px;"></div>
   <div style="width: 50px; height: 50px;"></div>
 </div>

 <div id="justify_content_min_max" style="justify-content: center; width: 100px; min-height: 100px; max-height: 200px; ">
   <div style="width: 60px; height: 60px;"></div>
 </div>

 <div id="align_items_min_max" style="align-items: center; height: 100px; min-width: 100px; max-width: 200px;">
   <div style="width: 60px; height: 60px;"></div>
 </div>

 <div id="align_items_stretch" style="width: 100px; height: 100px;">
   <div style="height: 10px;"></div>
 </div>

 <div id="align_items_center" style="align-items: center; width: 100px; height: 100px; ">
   <div style="width: 10px; height: 10px;"></div>
 </div>

 <div id="align_items_flex_start" style="align-items: flex-start; width: 100px; height: 100px; ">
   <div style="width: 10px; height: 10px;"></div>
 </div>

 <div id="align_items_flex_end" style="align-items: flex-end; width: 100px; height: 100px; ">
   <div style="width: 10px;height: 10px; "></div>
 </div>

 <div id="align_self_center" style="width:100px; height: 100px;">
   <div style=" align-self: center; width: 10px; height: 10px;"></div>
 </div>

 <div id="align_self_flex_end" style="width:100px; height: 100px;">
   <div style="align-self: flex-end; width: 10px; height: 10px; "></div>
 </div>

 <div id="align_self_flex_start" style="width:100px; height: 100px;">
   <div style="align-self: flex-start; width: 10px; height: 10px;"></div>
 </div>

 <div id="align_self_flex_end_override_flex_start" style="align-items: flex-start; width:100px; height: 100px; ">
   <div style="align-self: flex-end; width: 10px; height: 10px; "></div>
 </div>

 <div id="border_no_size" style="border-width: 10px;">
 </div>

 <div id="border_no_size_broken_out" style="border-left-width: 10px; border-right-width: 10px; border-top-width: 10px; border-bottom-width: 10px; ">
 </div>

 <div id="border_no_size_override_left" style="border-width: 20px; border-left-width: 10px;">
 </div>

 <div id="border_no_size_override_right" style="border-width: 20px; border-right-width: 10px;">
 </div>

 <div id="border_no_size_override_bottom" style="border-width: 20px; border-bottom-width: 10px;">
 </div>

 <div id="border_container_match_child" style="border-width: 10px;">
   <div style="width: 10px; height: 10px;"></div>
 </div>

 <div id="border_stretch_child" style="width: 100px; height: 100px; border-width: 10px;">
   <div style="height: 10px;"></div>
 </div>

 <div id="border_center_child" style="justify-content: center; align-items: center; width: 100px; height: 100px; border-start-width: 10px; border-top-width: 10px; border-end-width: 20px; border-bottom-width: 20px;  ">
   <div style="width: 10px;height: 10px;"></div>
 </div>

 <div id="max_width" style="width: 100px; height: 100px;">
   <div style="max-width: 50px; height: 10px; "></div>
 </div>

 <div id="max_height" style=" flex-direction: row; width: 100px; height: 100px;">
   <div style="width: 10px; max-height: 50px;"></div>
 </div>


 <div id="padding_no_size" style="padding: 10px;">
 </div>

 <div id="padding_container_match_child" style="padding: 10px;">
   <div style="width: 10px; height: 10px;"></div>
 </div>


 <div id="padding_stretch_child" style="width: 100px; height: 100px; padding: 10px;">
   <div style="height: 10px;"></div>
 </div>

 <div id="padding_center_child" style="justify-content: center; align-items: center; padding-top: 10px; padding-bottom: 20px; width: 100px; height: 100px; padding-start: 10px; padding-end: 20px; ">
   <div style="width: 10px; height: 10px; "></div>
 </div>

 <div id="absolute_layout_width_height_start_top" style="width: 100px; height: 100px;">
   <div style="position: absolute; top: 10px;start: 10px; width:10px; height: 10px;"></div>
 </div>

 <div id="absolute_layout_width_height_end_bottom" style="width: 100px; height: 100px;">
   <div style="position: absolute; bottom: 10px; end: 10px; width:10px; height: 10px;"></div>
 </div>

 <div id="absolute_layout_start_top_end_bottom" style="width: 100px; height: 100px;">
   <div style="position: absolute; start: 10px; top: 10px; end: 10px; bottom: 10px;"></div>
 </div>

 <div id="absolute_layout_width_height_start_top_end_bottom" style="width: 100px; height: 100px;">
   <div style="width:10px; height: 10px; position: absolute; start: 10px; top: 10px; end: 10px; bottom: 10px;"></div>
 </div>

 <div id="do_not_clamp_height_of_absolute_node_to_height_of_its_overflow_hidden_parent" style=" flex-direction: row; overflow: hidden; width: 50px; height: 50px; ">
   <div style="position: absolute; start: 0; top: 0;">
     <div style="width: 100px; height: 100px;"></div>
   </div>
 </div>

 <div id="flex_direction_column_no_height" style="width: 100px">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="flex_direction_row_no_width" style=" flex-direction: row; height: 100px;">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="flex_direction_column" style="width: 100px;height: 100px; ">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="flex_direction_row" style="flex-direction: row; width: 100px;height: 100px;  ">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="flex_direction_column_reverse" style=" flex-direction: column-reverse; width: 100px; height: 100px; ">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="flex_direction_row_reverse" style=" flex-direction: row-reverse; width: 100px; height: 100px; ">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="wrap_column" style="flex-wrap: wrap; width: 60px; height: 100px;">
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
 </div>

 <div id="wrap_row" style="flex-wrap: wrap; flex-direction: row; width: 100px;">
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
 </div>

 <div id="wrap_row_align_items_flex_end" style=" flex-direction: row; align-items: flex-end; flex-wrap: wrap; width: 100px;">
   <div style="width: 30px; height: 10px; "></div>
   <div style="width: 30px; height: 20px; "></div>
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
 </div>

 <div id="wrap_row_align_items_center" style=" flex-direction: row; align-items: center; flex-wrap: wrap; width: 100px;">
   <div style="width: 30px; height: 10px; "></div>
   <div style="width: 30px; height: 20px; "></div>
   <div style="width: 30px; height: 30px; "></div>
   <div style="width: 30px; height: 30px; "></div>
 </div>

 <div id="margin_start" style="flex-direction: row; width: 100px; height: 100px; ">
   <div style="width: 10px;  margin-start: 10px;"></div>
 </div>

 <div id="margin_end" style="flex-direction: row; justify-content: flex-end; width: 100px; height: 100px; ">
   <div style="width: 10px; margin-end: 10px;"></div>
 </div>

 <div id="margin_left" style=" flex-direction: row; width: 100px; height: 100px;">
   <div style="width: 10px; margin-left: 10px;"></div>
 </div>

 <div id="margin_top" style="width: 100px; height: 100px;">
   <div style="height: 10px; margin-top: 10px;"></div>
 </div>

 <div id="margin_right" style="flex-direction: row; justify-content: flex-end; width: 100px; height: 100px; ">
   <div style="width: 10px; margin-right: 10px;"></div>
 </div>

 <div id="margin_bottom" style=" justify-content: flex-end; width: 100px; height: 100px;">
   <div style="height: 10px; margin-bottom: 10px;"></div>
 </div>

 <div id="align_content_flex_start" style="flex-direction: column; align-content: flex-start; width: 100px; height: 100px; flex-wrap: wrap; ">
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
 </div>

 <div id="align_content_flex_end" style="flex-direction: column; align-content: flex-end;  flex-wrap: wrap; width: 100px; height: 100px;">
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
 </div>

 <div id="align_content_center" style=" flex-direction: column; align-content: center;  flex-wrap: wrap;width: 100px; height: 100px;">
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
   <div style="width: 50px; height: 10px;"></div>
 </div>

 <div id="align_content_stretch" style="flex-direction: column; align-content: stretch;flex-wrap: wrap; width: 100px; height: 100px; ">
   <div style="width: 50px;"></div>
   <div style="width: 50px;"></div>
   <div style="width: 50px;"></div>
   <div style="width: 50px;"></div>
   <div style="width: 50px;"></div>
 </div>

 <div id="justify_content_row_flex_start" style=" flex-direction: row; justify-content: flex-start; width: 102px; height: 102px;">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="justify_content_row_flex_end" style=" flex-direction: row; justify-content: flex-end; width: 102px; height: 102px;">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="justify_content_row_center" style="flex-direction: row; justify-content: center; width: 102px; height: 102px; ">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="justify_content_row_space_between" style=" flex-direction: row; justify-content: space-between; width: 102px; height: 102px;">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="justify_content_row_space_around" style=" flex-direction: row; justify-content: space-around; width: 102px; height: 102px;">
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
   <div style="width: 10px;"></div>
 </div>

 <div id="justify_content_column_flex_start" style="justify-content: flex-start; width: 102px; height: 102px; ">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="justify_content_column_flex_end" style="justify-content: flex-end; width: 102px; height: 102px; ">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="justify_content_column_center" style="justify-content: center;width: 102px; height: 102px; ">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="justify_content_column_space_between" style="justify-content: space-between; width: 102px; height: 102px; ">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="justify_content_column_space_around" style=" justify-content: space-around; width: 102px; height: 102px;">
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
   <div style="height: 10px;"></div>
 </div>

 <div id="border_flex_child" style="width: 100px; height: 100px; border-width: 10px;">
   <div style=" flex-grow:1; width: 10px;"></div>
 </div>

 <div id="min_height" style="width: 100px; height: 100px;">
   <div style="flex-grow: 1; min-height: 60px;"></div>
   <div style="flex-grow: 1;"></div>
 </div>

 <div id="min_width" style="flex-direction: row; width: 100px; height: 100px; ">
   <div style="flex-grow: 1; min-width: 60px;"></div>
   <div style="flex-grow: 1;"></div>
 </div>
 <div id="padding_flex_child" style="width: 100px; height: 100px; padding: 10px;">
   <div style=" flex-grow:1; width: 10px;"></div>
 </div>
 <div id="margin_and_flex_row" style=" flex-direction: row; width: 100px; height: 100px;">
   <div style=" flex-grow: 1; margin-start: 10px; margin-end: 10px;"></div>
 </div>

 <div id="margin_and_flex_column" style="width: 100px; height: 100px;">
   <div style=" flex-grow: 1; margin-top: 10px; margin-bottom: 10px;"></div>
 </div>

 <div id="margin_and_stretch_row" style=" flex-direction: row; width: 100px; height: 100px;">
   <div style=" flex-grow: 1; margin-top: 10px; margin-bottom: 10px;"></div>
 </div>

 <div id="margin_and_stretch_column" style="width: 100px; height: 100px;">
   <div style=" flex-grow: 1; margin-start: 10px; margin-end: 10px;"></div>
 </div>

 <div id="margin_with_sibling_row" style=" flex-direction: row; width: 100px; height: 100px;">
   <div style=" flex-grow: 1; margin-end: 10px;"></div>
   <div style="flex-grow: 1;"></div>
 </div>

 <div id="margin_with_sibling_column" style="width: 100px; height: 100px;">
   <div style=" flex-grow: 1; margin-bottom: 10px;"></div>
   <div style="flex-grow: 1;"></div>
 </div>
 <div id="flex_basis_flex_grow_column" style="width: 100px; height: 100px;">
   <div style=" flex-grow: 1; flex-basis: 50px;"></div>
   <div style="flex-grow: 1;"></div>
 </div>

 <div id="flex_basis_flex_grow_row" style=" flex-direction: row; width: 100px; height: 100px;">
   <div style=" flex-grow: 1; flex-basis: 50px;"></div>
   <div style="flex-grow: 1;"></div>
 </div>

 <div id="flex_basis_flex_shrink_column" style="width: 100px; height: 100px;">
   <div style="flex-basis: 100px; flex-shrink: 1;"></div>
   <div style="flex-basis: 50px;"></div>
 </div>

 <div id="flex_basis_flex_shrink_row" style=" flex-direction: row; width: 100px; height: 100px;">
   <div style="flex-basis: 100px; flex-shrink: 1;"></div>
   <div style="flex-basis: 50px;"></div>
 </div>

 <div id="flex_basis_flex_grow_undefined_main" style="width: 100px;">
   <div style=" flex-grow: 1; flex-basis: 100px;"></div>
   <div style="flex-basis: 50px;"></div>
 </div>

 <div id="flex_basis_flex_grow_undefined_main_w_margin_top" style="width: 100px; margin-top:10px;">
   <div style=" flex-grow: 1; flex-basis: 100px;"></div>
   <div style="flex-basis: 50px;"></div>
 </div>

 <div id="flex_basis_flex_grow_undefined_main_w_margin_bottom" style="width: 100px; margin-bottom:10px;">
   <div style=" flex-grow: 1; flex-basis: 100px;"></div>
   <div style="flex-basis: 50px;"></div>
 </div>

 <div id="flex_basis_flex_grow_undefined_main_w_margin_left" style="width: 100px; margin-left:10px;">
   <div style=" flex-grow: 1; flex-basis: 100px;"></div>
   <div style="flex-basis: 50px;"></div>
 </div>

 <div id="jwalke_border_width_only_start" style=" align-items: center; justify-content: center; width: 100px; height: 100px; border-start-width: 10px; border-top-width: 10px; border-bottom-width: 20px;">
   <div style="height: 10px; width: 10px;"></div>
 </div>

 <div id="jwalke_border_width_only_end" style="align-items: center; justify-content: center; width: 100px; height: 100px; border-end-width: 10px; border-top-width: 10px; border-bottom-width: 20px; ">
   <div style="height: 10px; width: 10px;"></div>
 </div>


 <div id="start_overrides_margin" style="width: 100px; height: 100px;">
   <div style="
     flex-grow: 1;
     margin-left: 20px;
     margin-right: 20px;
     margin-start: 10px;"
   >
   </div>
 </div>

 <div id="end_overrides_margin" style="width: 100px; height: 100px;">
   <div style="
     flex-grow: 1;
     margin-left: 20px;
     margin-right: 20px;
     margin-end: 10px;"
   >
   </div>
 </div>

 <div id="start_overrides_padding" style="width: 100px; height: 100px;">
   <div style="
     flex-grow: 1;
     padding-left: 20px;
     padding-right: 20px;
     padding-start: 10px;"
   >
   </div>
 </div>

 <div id="end_overrides_padding" style="width: 100px; height: 100px;">
   <div style="
     flex-grow: 1;
     padding-left: 20px;
     padding-right: 20px;
     padding-end: 10px;"
   >
   </div>
 </div>

 <div id="start_overrides_border" style="width: 100px; height: 100px;">
   <div style="
     flex-grow: 1;
     border-left-width: 20px;
     border-right-width: 20px;
     border-start-width: 10px;"
   >
   </div>
 </div>

 <div id="end_overrides_border" style="width: 100px; height: 100px;">
   <div style="
     border-left-width: 20px;
     border-right-width: 20px;
     border-end-width: 10px;
     flex-grow: 1;"
   >
   </div>
 </div>

 <div id="start_overrides" style="width: 100px; height: 100px;">
   <div style="
     position:absolute;
     left: 20px;
     right: 20px;
     start: 10px;
     flex-grow: 1;"
   >
   </div>
 </div>

 <div id="end_overrides" style="width: 100px; height: 100px;">
   <div style="
     position:absolute;
     left: 20px;
     right: 20px;
     end: 10px;
     flex-grow: 1;"
   >
   </div>
 </div>

 <div id="flex_shrink_to_zero" style="height: 75px;">
   <div style="width: 50px; height: 50px; flex-shrink:0;"></div>
   <div style="width: 50px; height: 50px; flex-shrink:1;"></div>
   <div style="width: 50px; height: 50px; flex-shrink:0;"></div>
 </div>

 <div id="flex_basis_overrides_main_size" style="height: 100px; width: 100px;">
   <div style="height: 20px; flex-grow:1; flex-basis:50px;"></div>
   <div style="height: 10px; flex-grow:1;"></div>
   <div style="height: 10px; flex-grow:1;"></div>
 </div>

<div id="flex_grow_shrink_at_most" style="height: 100px; width: 100px;">
  <div>
    <div style="flex-grow:1; flex-shrink:1;"></div>
  </div>
</div>
 
<div id='bugRepro' style='align-items:auto; padding-start: 48px; padding-end:48px; width:1408px; flex-direction: row;'>
  <div style=''>
    <div style=''></div>
  </div>
  <div style=''>
    <div style=''>
      <div style=''></div>
    </div>
  </div>
</div>


<div id='bugRepro2' style='padding-start: 10px; padding-end:10px; width:200px; flex-direction: row;'>
  <div style='justify-content:flex-start; width:20px;'>
  </div>
  <div style='min-height: 0px; flex-direction: row; flex-grow: 1;'>
    <div style='flex-grow: 1;'> </div>
  </div>
</div>

*/





/*

This one doesn't work correctly in Chrome, so not sure what to do.

 <div id="wrap_column" style="height: 100px; flex-wrap: wrap">
   <div style="height: 30px; width: 30px;"></div>
   <div style="height: 30px; width: 30px;"></div>
   <div style="height: 30px; width: 30px;"></div>
   <div style="height: 30px; width: 30px;"></div>
 </div>

*/

