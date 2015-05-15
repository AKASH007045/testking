<div class="open-game-popup <?php  print isset($event['class']) ? $event['class'] : ''; ?> sth-calendar-event" <?php print $attributes; ?>>
<a class="inline" data-target="#modal_bootstrap" data-toggle="modal" href="#cal-event-<?php print $node->nid; ?>" ga-title="<?php  print $node->title; ?>">
  <div class="team-info event">
    <div class="field field-name-field-image field-type-image field-label-hidden" data-id="cal-event-<?php print $node->nid; ?>"><div class="field-items"><div class="field-item even">
     <img src="/sites/all/themes/sports/images/bg-event-legend.png" />
    </div></div></div>
  </div>
  </a>
</div>

<div id="cal-event-<?php print $node->nid; ?>" class="modal fade game-popup sth-cal-poup" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
          <div class="poupatstud"><?php print (isset($full_title) && !empty($full_title))?$full_title:$title; ?></span></div>
          <div class="pptic-evt">
              <?php /* if ($rsvp): ?><div class="mgTicket"><a href="javascript:void(0);" class="rsvp">RSVP</a></div> <?php endif; */?>
              <div class="mInfo"><a href="javascript:void(0);" class="more-info">MORE INFO</a></div>
              <div class="closePopup"></div>
        </div>
      </div>
      <div class="modal-body">
          <div class="pouptopwrap">
            <div class="sthevent-image">
                <?php print $event['image'];?>
                
                <div class="sthevent-body more-info" id="more-info"><?php  print $event['description']; ?></div>
                <?php /* if ($rsvp): ?>
                <div class="sthevent-form rsvp-form" id="rsvp">
                  <iframe src="/rsvp-form" width="980" height="400" scrolling="no"></iframe>
               <a href="javascript:void(0);" class="upArrow">
                <div></div></a>
              </div>
            <?php endif; */?>
            </div>            
          </div>
      </div>      
    </div>
  </div>  
</div>
