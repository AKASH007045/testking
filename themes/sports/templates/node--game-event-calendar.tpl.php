<?php $identifier = ($event['class'] == 'game-home') ? 'VS' : '@';
$game_result = array();
if ((isset($home_score) && isset($other_score)) && (($home_score || $other_score)) && $game_over) {
    $game_result['result'] = 1;
     if ($home_score > $other_score) {
       $game_result['score'] = '<span class="final-result">W </span><span class="final-score">&nbsp;' . $home_score . '-' . $other_score . '</span>';
     }
     else {
      $game_result['score'] = '<span class="final-result">L </span><span class="final-score">&nbsp;' . $home_score . '-' . $other_score . '</span>';
     }
   }
   else {
    $game_result['result'] = 0;
   }
?>
<div class="open-game-popup <?php  print $event['class']; ?>" <?php print $attributes; ?>>
  <div class="team-info">
    <?php
   if (!$game_result['result']): ?><a class="inline" data-target="#modal_bootstrap" data-toggle="modal" href="#cal-event-<?php print $node->nid; ?>" ga-title="<?php  print $identifier; ?> <?php print $other_team_abbr;?>"> <?php endif; ?>
    <div class="field field-name-field-image field-type-image field-label-hidden" data-id="cal-event-<?php print $node->nid; ?>"><div class="field-items"><div class="field-item even">
    <?php
     print '<div class="desktop-logo">' . $other_team . '</div>';
     print '<div class="iPad-logo">' . $ipadother_team . '</div>';
  ?>
    </div></div></div>
  <?php if ((!$game_result['result'])) { ?></a>
    <div class="cal-team-name"><?php  print $identifier; ?> <?php print $other_team_abbr;?></div><div class="cal-time"><?php print $event['time'];?></div>    <?php } else {?>
    <div class="cal-game-result"><?php print $game_result['score'];?></div>
    <?php } ?>
  </div>  
</div>

<div id="cal-event-<?php print $node->nid; ?>" class="modal fade game-popup sth-cal-poup" role="dialog" aria-hidden="true">
    <div class="modal-dialog game-popup-dialog">
    <div class="modal-content">
      <div class="modal-header">
          <div class="poupatstud"><?php print $event['stadium'];?> - <span><?php print $event['date_1'];?> </span> - <span><?php print $event['time'];?></span> <span><?php print $event['timezone']; ?></span></div>
          <div class="pptic-evt">
<<<<<<< HEAD
              <?php if (user_access('STH User') && $identifier == 'VS'): ?><div class="mgTicket"><a href="/goto-account-manager" target="_blank">MANAGE TICKETS</a></div>
              <?php endif; ?>
=======
              <?php if (user_access('STH User') && $identifier == 'VS'): ?><div class="mgTicket"><a href="/goto-account-manager" target="_blank">MANAGE TICKETS</a></div> 
			  <?php endif; ?>
>>>>>>> develop1.2.0.0
              <!--<div><a href="#" target="_blank">MORE INFO</a></div>-->
              <div class="closePopup btn btn-default" data-dismiss="modal"></div>
        </div>
      </div>
      <div class="modal-body">
          <div class="pouptopwrap">
            <div class="toplogolft"><?php print $home_team_bg;?></div>
            <div class="topvscnt"><?php  print $identifier; ?></div>
            <div class="toplogorgt"><?php print $other_team_bg;?></div>
          </div>
      </div>      
    </div>
  </div>  
</div>
