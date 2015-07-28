<?php $class = (arg(1) == $node->nid) ? 'active' : ''; ?>
<aside class="event-slider-box <?php print $class;?>" <?php print $attributes; ?>>
<div class="tilehead">
  <span><a id="<?php print $node->nid; ?>" href="javascript:void(0);"><?php print $title; ?></a></span>
</div>
<?php
if (user_access('edit any event content')) {
print '<div class="event-link-edit">'.l('<i class="fa fa-edit"></i> Edit', 'node/'.$node->nid.'/edit', array('html'=>'true')).'</div>';
}
?>
  <div id="gallery-slider"><a id="<?php print $node->nid; ?>" href="javascript:void(0);">
    <div class="first"><?php print $image;?></div>
</a></div>
</aside>