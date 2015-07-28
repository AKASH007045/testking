<div class="panel panel-default <?php print $class; ?>" id="<?php print $class; ?>">
    <div class="panel-heading">
        <h4 class="panel-title">
            <a class="accordion-toggle collapsed" data-toggle="collapse" data-parent="#accordion" href="#collapse<?php print $count;?>">
            <span class="list-view-child"><?php print $data['day']?></span>
            <?php if ($data['result']) { print $data['score']; } else {?>
                <span class="list-view-child"><?php print $data['time']?></span>
                <?php }?>
                <span class="list-view-child"><?php print $data['sym']?></span>
                <span class="list-view-child">
                    <?php print $data['team_details']['listlogo'];?>
                </span>
                <span class="list-view-child list-view-toggle"><i class="fa fa-angle-down"></i></span>
            </a>
        </h4>
    </div>
    <div id="collapse<?php print $count;?>" class="panel-collapse collapse">
        <div class="panel-body">
            <span class="content-wrap-child toplogolft">
                <?php print $home_team['overlay_image'];?>
            </span>
            <span class="content-wrap-child topvscnt"><?php print $data['sym']?></span>
            <span class="content-wrap-child toplogorgt">
                <?php print $data['team_details']['overlay_image'];?>
            </span>
            <span class="bottom-text-wrap"><?php print $data['stadium']?> <?php print $data['time']?></span>
        </div>
    </div>
</div>