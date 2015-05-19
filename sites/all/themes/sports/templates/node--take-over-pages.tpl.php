<?php
/**
 * @file
 * Default theme implementation to display a node.
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
 *
 * @ingroup themeable
 */
//print '<pre>';
//print_r($node);
//exit; 

drupal_add_css(drupal_get_path('module', 'custom_take_over_pages') . '/css/take_over.css');
?>
<?php if (user_access('Editor')) { ?>
    <div class="stp-editor-links highlighted">
        <div class="tabs-wrap-primary-user"><div class="tabsoption-primary-user">
                <div class="home-tgl"><div class="wrapper-icon-sidekick"><i class="fa fa-angle-right"></i><span class="sidekick-label"></span></div></div>
                <ul class="page-name"><li>Page:</li> <li><?php print $node->title; ?></li></ul>
            </div>
            <ul class="tabs-primary-live nav"><li class="node-view"><?php print l('<i class="fa fa-eye"></i> View', 'node/' . $node->nid, array('attributes' => array('class' => array('active')), 'html' => TRUE)); ?></li>
                <li data-id="node-<?php print $node->nid; ?>" data-type="<?php print $node->type; ?>" class="edit-link"><a href="#"><i class="fa fa-edit"></i> Edit</a></li>
                <?php if (isset($tab_page) && $tab_page): ?>
                    <li class="delete-link"><a href="/node/<?php print $node->nid; ?>/delete?destination=node/<?php print $tab_page; ?>"><i class="fa fa-trash-o"></i> Delete</a></li>
                <?php endif; ?>
            </ul>
            <?php if (isset($tab_page) && $tab_page): ?>
                <a href="/node/add/pdf/<?php print $tab_page ?>?destination=node/<?php print $tab_page; ?>"><button class="view-versions-btn">Add PDF Tab</button></a>
                <a href="/node/add/stp-page/<?php print $tab_page ?>?destination=node/<?php print $tab_page; ?>"><button class="view-versions-btn">Add Content Tab</button></a>
              <!--<ul class="tabs-primary-live nav"><li data-id="node-<?php print $tab_page; ?>" data-type="<?php print $node->type; ?>" class="edit-link"><a href="#">Edit Page</a></li></ul>-->
                <a href="/node/add/tab-page"><button class="view-versions-btn">Add Tabbed Page</button></a>
            <?php endif; ?>
        <!--<a href="/node/<?php print $node->nid ?>/revisions"><button class="view-versions-btn">View Versions</button></a>-->
            <!-- <button class="publish-btn">Publish</button>-->
        </div>
    </div>
<?php } ?>


<?php

module_load_include('inc', 'node', 'node.pages');
form_load_include($form_state, 'inc', 'node', 'node.pages');

$node1 = (object) node_load($node->nid);
/*$form_state['build_info']['args'] = array($node1);
$form_state['#']['args'] = array($node1);
$form_id = 'take_over_pages_node_form';
$form = drupal_build_form('take_over_pages_node_form', $form_state);*/
$form['title']['#maxlength'] = '20';

unset($form['field_tags']);
unset($form['revision_information']['#access']);
//unset($form['#after_build'][] = 'custom_after_build';
unset($form['comment_settings']);
unset($form['author']);
unset($form['translations']);
unset($form['options']);
unset($form['path']);
unset($form['menu']);
unset($form['actions']['delete']);
unset($form['actions']['preview']);


$from = render($form);
print '<div style="display: none;"  class="take_over_form take-over-pages-form take-over-pages-form-' . $node->nid . '"><div id="my-form-wrapper-' . $nid . '">' . $from . '</div></div>';
?>


<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <?php if ((!$page && !empty($title)) || !empty($title_prefix) || !empty($title_suffix) || $display_submitted): ?>
        <header>
            <?php print render($title_prefix); ?>
            <?php if (!$page && !empty($title)): ?>
                <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
        </header>
    <?php endif; ?>
    <?php if($node->field_select_layout[$node->language][0]['value'] != 3): ?>
    <div class="to-body"><?php print render($content['body']); ?></div>
    <div class="takeover-btn-wrap">
        <div class="to-btn1 tk-btn button_blue"><?php print render($content['field_button_1']); ?></div>
        <?php if ($node->field_select_layout[$node->language][0]['value'] == 2) { ?>
            <div class="to-btn2 tk-btn button_blue"><?php print render($content['field_button_2']); ?></div>
        <?php } ?>
    </div>
    <div class="to-btm-txt"><?php print render($content['field_bottom_text']); ?></div>
    <div class="to-bg-img"><?php print render($content['field_bg_image']); ?></div>
    <?php
     elseif($node->field_select_layout[$node->language][0]['value'] == 3):
    ?>
    <div class="to-body"><?php print render($content['field_video']);?></div>
    <div class="to-btm-txt"><?php print render($content['field_bottom_text']); ?></div>
    <?php endif;?>
    
</article>
