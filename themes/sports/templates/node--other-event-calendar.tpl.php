<div class="sth-other-event 3rdparty-event" <?php print $attributes; ?>>
<?php if (isset($node->field_link['und'][0]['url'])): ?>
<a target="_blank" href="<?php print (isset($node->field_link['und'][0]['url'])) ? $node->field_link['und'][0]['url'] : 'javascript:void(0)';?>" ga-title="<?php print $node->title;?>"><?php endif;?>
<?php
     print '<div class="desktop-logo">' .  $event['image'] . '</div>';
     //print '<div class="iPad-logo">' .  $event['iPadimage'] . '</div>';
     print '<div class="iPad-logo">' .  $event['image'] . '</div>';
  ?>
<?php if (isset($node->field_link['und'][0]['url'])): ?></a><?php endif; ?></div>