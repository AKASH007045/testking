<?php

/**
 * @file
 * Bartik's theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 
 */

?>
<div class="home-page-tile bottom-promo-tile col-lg-3 col-md-3 col-sm-3 col-xs-3 <?php print $classes; ?>" <?php print $attributes; ?> id="promo-node-<?php print $node->nid;?>">

    <?php if (user_access('edit any promo_tile content')) {?>   
      <a class="promo-link-edit" href="javascript://" >edit</a>
      <?php
          module_load_include('inc', 'node', 'node.pages');
          form_load_include($form_state, 'inc', 'node', 'node.pages');
        //  $nid = arg(4);
        //$form_state = array(
         // $form_state['ajax'] = TRUE;
        //  'title' => 'Promotional Tiles',
        //);
          $node1 = (object)node_load($node->nid);
          $form_state['build_info']['args'] = array($node1);
           $form_state['#']['args'] = array($node1);
          $form_id = 'promo_tile_node_form';
          
        //  $form =   drupal_retrieve_form($form_id, $form_state);
        //  drupal_prepare_form($form_id, $form, $form_state);
        //   drupal_process_form($form_id, $form, $form_state);
         // flog_it($form);
         $form = drupal_build_form('promo_tile_node_form', $form_state);
    
         //$form['#action'] = $_GET['q'] . '?destination=' . $_GET['q'];
       
         //print_r($form);
       //  exit;
         $from = render($form);
        ;print '<div class="tile-image-form tile-image-form-'.  $node->nid . '"><i class="edit-toolbar-pointer"></i><div class="title"><span class="titletxt">Promotional Tiles</span> <a class="popup-close" href="javascript://" >&nbsp;</a></div><div id="my-form-wrapper-' . $nid . '">'. $from . '</div></div>'; 
      }
      ?>
<aside class="promo-box <?php print $tile_type;?>">
  <div class="tilehead">
    <span><?php print $title;?></span>
  </div>
<a class="promo-link" href="<?php print (isset($node->field_link['und'][0]['url'])) ? $node->field_link['und'][0]['url'] : 'javascript:void(0)';?>" target="<?php print isset($node->field_link['und'][0]['attributes']['target']) ? $node->field_link['und'][0]['attributes']['target'] : '_self';?>">
   <div class="first"><?php print render($content['field_image']);?></div>
   <div class="second"><?php //print render($content['field_gray_image']);?></div>
  <?php if (isset($daysleft) && $daysleft && $tile_type == 'countdown'):?>
    <div class="promo-countdown"><div class="countdown-promo-tile-days"><?php print $daysleft;?></div><div class="countdown-promo-tile-title"><?php print $promo_title;?></div></div>
  <?php  endif;?>
    <?php /* if ($is_front) {?>
      <div class="first"><?php print render($content['field_image']);?></div>
      <div class="second"><?php print render($content['field_gray_image']);?></div>
    <?php } else { ?>
      <div class="first"><?php print render($content['field_gray_image']);?></div>
      <div class="second"><?php print render($content['field_image']);?></div>
    <?php } */?>
</a>
  </aside>
</div>
