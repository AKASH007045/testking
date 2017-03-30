

<?php

  // Render or hide parts of $form: var_export($form);
  // Example given:
  //hide($form['title']);   
  // Render remaining form elements as usual.
  print drupal_render_children($form);
?>
