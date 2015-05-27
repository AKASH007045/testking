<?php
/**
 * @file
 * theme functions for the Custom Crop module.
 */

 /**
 * Returns HTML for a summary of an image crop and scale effect.
 *
 * @param $variables
 *   An associative array containing:
 *   - data: The current configuration for this crop and scale effect.
 *
 * @ingroup themeable
 */
function theme_custom_crop_crop_and_scale_summary($variables) {
  $data = $variables["data"];
  $str = check_plain($data['width']) . 'x' . check_plain($data['height']);

  if ($data['upscale'] || $data['respectminimum'] || !empty($data['respectminimum'])) {
    $str .= ' (' . ($data['upscale'] ? t('upscaling allowed') : '');

    if ($data['upscale'] && ($data['respectminimum'] || !empty($data['onlyscaleifcrop']))) {
      $str .= ($data['respectminimum'] && !empty($data['onlyscaleifcrop']) ? ', ' : ' ' . t('and') . ' ');
    }

    if ($data['respectminimum']) {
      $str .= t('crop minimum dimensions') . (!empty($data['onlyscaleifcrop']) ? ' ' . t('and') . ' ' : '');
    }

    if (!empty($data['onlyscaleifcrop'])) {
      $str .= t('only scale if cropped');
    }

    $str .= ')';
  }

  return $str;
}

/**
 * Returns HTML for a summary of an image crop effect.
 *
 * @param $variables
 *   An associative array containing:
 *   - data: The current configuration for this crop effect.
 *
 * @ingroup themeable
 */
function theme_custom_crop_crop_summary($variables) {
  $data = $variables['data'];

  if ($data['width'] && $data['height']) {
    $str = ' (' . t('minimum crop dimensions ') . ' ' . check_plain($data['width']) . 'x' . check_plain($data['height']);

    if (!empty($data['keepproportions'])) {
      $str .= ' ' . t('and') . ' ' . ($data['keepproportions'] ? t('maintain proportions') : '');
    }

    return $str . ')';
  }
  elseif ($data['width'] || $data['height']) {
    return ' (' . ($data['width']) ? t('minimum crop width @width', array('@width' => $data['width'])) : t('minimum crop height @height', array('@height' => $data['height'])) . ')';
  }

}

/**
 * Returns HTML for a summary of an image crop selection reuse effect.
 *
 * @param $variables
 *   An associative array containing:
 *   - data: The current configuration for this crop selection reuse effect.
 *
 * @ingroup themeable
 */
function theme_custom_crop_reuse_summary($variables) {
  return '(' . t('using @style', array('@style' => _custom_crop_image_style_name($variables['data']['reusestyle']))) . ')';
}

/**
 * Returns HTML for the overlay crop area of an image.
 *
 * @param $variables
 *   An associative array containing:
 *   - "attributes": An array of attributes.
 *   - "image": An array of variables for the image theming function.
 *
 * @ingroup themeable
 */
function theme_custom_crop_croptool_overlay($variables) {
  $output = '<div ' . drupal_attributes($variables['attributes']) . '>';

  $output .= '<div class="custom_crop-overlay-bg"></div>';
  $output .= '<div class="custom_crop-image-title">' . t('Adjust Headshot'). '</div>';
  $output .= '<div class="custom_crop-image-holder">';
  $output .= theme('image', $variables["image"]);
  $output .= '</div>';

  if ($variables['instant_preview']) {
    $output .= '<div class="custom_crop-instantpreview"></div>';
  }

  if ($variables['crop_info']) {
    $output .= '<div class="custom_crop-details">';
   // $output .= t('Image style') . ': <span class="custom_crop-details-value custom_crop-image-style">&nbsp;</span>, ';
    $output .= t('Selection x') . ': <span class="custom_crop-details-value custom_crop-selection-x">-</span>, ';
    $output .= t('Selection y') . ': <span class="custom_crop-details-value custom_crop-selection-y">-</span>, ';
    $output .= t('Selection width') . ': <span class="custom_crop-details-value custom_crop-selection-width">-</span>, ';
    $output .= t('Selection height') . ': <span class="custom_crop-details-value custom_crop-selection-height">-</span>';
    $output .= '</div>';
  }

  $output .= '<div class="custom_crop-buttons">';
  $output .= '<a class="custom_crop-button custom_crop-reset" href="javascript:void(0);" onmousedown="custom_crop.closeCroptool(true);">' . t('Cancel') . '</a>';
  //$output .= '<a class="custom_crop-button custom_crop-maximize" href="javascript:void(0);" onmousedown="custom_crop.maximizeSelection();">' . t('Maximize selection') . '</a>';
  //$output .= '<a class="custom_crop-button custom_crop-clear" href="javascript:void(0);" onmousedown="custom_crop.clearSelection();">' . t('Remove selection') . '</a>';
  //$output .= '<a class="custom_crop-button custom_crop-reset" href="javascript:void(0);" onmousedown="custom_crop.resetSelection();">' . t('Revert selection') . '</a>';
  $output .= '<a class="custom_crop-button custom_crop-close" href="javascript:void(0);" onmousedown="custom_crop.closeCroptool();">' . t('Save') . '</a>';
  $output .= '</div>';

  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for the inline crop area of an image.
 *
 * @param $variables
 *   An associative array containing:
 *   - "attributes": An array of attributes.
 *   - "image": An array of variables for the image theming function.
 *
 * @ingroup themeable
 */
function theme_custom_crop_croptool_inline($variables) {
  $output = '<div ' . drupal_attributes($variables['attributes']) . '>';

  $output .= '<div class="custom_crop-image-holder">';
  $output .= theme('image', $variables['image']);
  $output .= '</div>';

  $output .= '<div class="clearfix">';

  if ($variables['instant_preview']) {
    $output .= '<div class="custom_crop-instantpreview"></div>';
  }

  if ($variables['crop_info']) {
    $output .= '<div class="custom_crop-details">';
   // $output .= t('Image style') . ': <span class="custom_crop-details-value custom_crop-image-style">&nbsp;</span><br />';
    $output .= t('Selection x') . ': <span class="custom_crop-details-value custom_crop-selection-x">-</span><br />';
    $output .= t('Selection y') . ': <span class="custom_crop-details-value custom_crop-selection-y">-</span><br />';
    $output .= t('Selection width') . ': <span class="custom_crop-details-value custom_crop-selection-width">-</span><br />';
    $output .= t('Selection height') . ': <span class="custom_crop-details-value custom_crop-selection-height">-</span>';
    $output .= '</div>';
  }

  $output .= '<div class="custom_crop-buttons">';
  $output .= '<input type="button" value="' . t('Save') . '" class="custom_crop-button custom_crop-close form-submit" onmousedown="custom_crop.closeCroptool();" />';
  $output .= '<input type="button" value="' . t('Revert selection') . '" class="custom_crop-button custom_crop-reset form-submit" onmousedown="custom_crop.resetSelection();" />';
  $output .= '<input type="button" value="' . t('Remove selection') . '" class="custom_crop-button custom_crop-clear form-submit" onmousedown="custom_crop.clearSelection();" />';
  $output .= '<input type="button" value="' . t('Maximize selection') . '" class="custom_crop-button custom_crop-maximize form-submit" onmousedown="custom_crop.maximizeSelection();" />';
  $output .= '<input type="button" value="' . t('Cancel') . '" class="custom_crop-button custom_crop-cencel form-submit" onmousedown="custom_crop.closeCroptool(true);" />';
  $output .= '</div>';

  $output .= '</div>';

  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for thumbnail list.
 *
 * @param $variables
 *   An associative array containing:
 *   - "attributes": An array of attributes.
 *   - "images": Array of images html, themed by the "custom_crop_thumblist_image" function an keyed by style name.
 *
 * @ingroup themeable
 */
function theme_custom_crop_thumblist($variables) {
  $output = '<div ' . drupal_attributes($variables['attributes']) . '>';
  $output .= '<div class="clearfix">';
  $output .= implode('', $variables['images']);
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for a single image in the thumbnail list.
 *
 * @param $variables
 *   An associative array containing:
 *   - "attributes": An array of attributes.
 *   - "image": Rendered image tag html.
 *   - "style": Clean style name.
 *
 * @ingroup themeable
 */
function theme_custom_crop_thumblist_image($variables) {
  $output = '<a ' . drupal_attributes($variables['attributes']) . '>';
  $output .= '<strong class="custom_crop-style-thumb-label">' . check_plain($variables['style']) . '</strong>';
  $output .= $variables['image'];
  $output .= '</a>';

  return $output;
}
