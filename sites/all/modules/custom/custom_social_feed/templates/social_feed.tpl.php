<?php

/**
 * @file
 * Theme template for a list of tweets.
 *
 * Available variables in the theme include:
 *
 * 1) An array of $tweets, where each tweet object has:
 *   $tweet->id
 *   $tweet->username
 *   $tweet->userphoto
 *   $tweet->text
 *   $tweet->timestamp
 *   $tweet->time_ago
 *
 * 2) $twitkey string containing initial keyword.
 *
 * 3) $title
 *
 */
 $filter = new stdClass();
  $filter->settings = array(
    'filter_url_length' => 80,
  );
?>
<aside class="promo-box">
<div class="tilehead social-icons"><span>SOCIAL FEED</span>
    <div>
    <a class="social" id="instagram"><i class="fa fa-instagram"></i></a><a id="twitter" class="social"><i class="fa fa-twitter"></i></a> <a id="facebook" class="social active"> <i class="fa fa-facebook-square"></i></a>
    </div>
</div>

<div class="abc" style="width:100%; margin: 0 auto; height:193px;">
<?php
$logo = array('twitter' => 'twitter-feed-logo.png', 'facebook' => 'facebook-feed-logo.png', 'instagram' => 'instagram-feed-logo.png');
if (!empty($feed)):
  foreach ($feed as $key => $data): ?>
    <div id="carousel-<?php print $key;?>" class="carousel slide">
    <div class="carousel-inner">
    <?php
    if (!empty($data)) {
      $i = 0;
      foreach ($data as $item):
        $active = ($i ==0) ? 'active' : '';
	if (isset($item->text) && $item->text) {?>
	  <div class="feed-details <?php print $active;?> item">
	    <div class="feed-logo">
	      <img src="/sites/all/modules/custom/custom_social_feed/images/<?php print $logo[$key];?>" alt="twitter"></div> <div class="feedtime"> - <?php print format_interval((time() - $item->timestamp) , 2) . t(' ago'); ?></div>
	      <div class="feedtex"><?php print ($key == 'twitter') ? twitter_pull_add_links($item->text) : '<a class="text-white" target="_blank" href="http://www.'. $key .'.com/sacramentokings">' . $item->text . '</a>';?></div>
         </div>
	<?php
	  $i++;
	}
      endforeach;
    } ?>
    </div>
  </div>
  <?php endforeach; endif; ?>
  
  </div>
</aside>
