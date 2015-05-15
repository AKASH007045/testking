<?php

if (arg(1) != 'add'){
 echo  '<div class="node-buttons">';
 echo l('Add Account Rep', 'node/add/acct-rep');
 echo '</div>';
}

//commented by debraj on 6/11/2014
//echo views_embed_view('acct_reps','block_3');

?>
<div class="node-add-wrapper clear-block acc-rep-form">

  <div class="node-column-sidebar">
    <?php if($sidebar): ?>
      <?php print render($sidebar); ?>
    <?php endif; ?>
  </div>
  <div class="node-column-main">
    <?php if($form): ?>
      <?php print drupal_render_children($form); ?>
    <?php endif; ?>
    
  </div>
<?php if($buttons): ?>
      <div class="node-buttons">
        <?php print render($buttons); 
		?>
		
    </div>
  <?php endif; ?>    
  <div class="clear"></div>
   
</div>
