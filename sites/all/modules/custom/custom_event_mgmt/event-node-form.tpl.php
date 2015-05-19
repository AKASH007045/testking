<div class="node-add-wrapper clear-block event-gallery-form123">
<?php if(arg(1) != 'add'): ?>
      <div class="node-buttons">
        <?php 
		 //print render($buttons);
		 print l(' Add New STH Event ', 'node/add/event');		
		?>
		
    </div>
  <?php endif; ?>
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
        <?php print render($buttons);?>
		
    </div>
  <?php endif; ?>
  <div class="clear"></div>
 
</div>