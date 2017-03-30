 <link type="text/css" rel="stylesheet" href="/<?php print drupal_get_path('theme', 'sports') . '/mail.css';?>" media="all" />
<?php if(isset($node->body['und'][0]['value'])){ print $node->body['und'][0]['value'];} else{
      print "No Preview";
    }?>