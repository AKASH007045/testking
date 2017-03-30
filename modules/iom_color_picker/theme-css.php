<?php

/*****************************Theme Sass***************/


$css = "

/*-------login page button start-------------*/
.page-user .btn-default,
.page-user .btn-default:focus,
.having-trouble-box .button_blue a,
.having-trouble-box .button_blue a:focus {
    color: $buttons_color; 
    border: 1px solid $buttons_border_color;
    box-shadow: 0px 1px 0px $buttons_boxshadow_color inset;  
    -webkit-box-shadow: 0px 1px 0px $buttons_boxshadow_color inset; 
    $buttons_default_color_gradiant
}
.page-user .btn-default:hover,
.having-trouble-box .button_blue a:hover {
    background: $buttons_bg_hover_color;
    border: 1px solid $buttons_border_hover_color;
    color: $buttons_text_hover_color;
}
/*-------Global page button start-------------*/
.button_blue a,
#renewal-countdown-timer #edit-actions .form-submit,
.global-button,
.cbtn button,
.global-button a,
.page-pawp .container-fluid .main-wrap section.block button.btn-default,
.page-pawp .container-fluid .main-wrap section.block button.btn-default:focus,
.page-pawp .container-fluid #user-login #edit-actions .form-submit,
.page-pawp .container-fluid #user-login #edit-actions .form-submit:focus,
.takeover-btn-wrap .tk-btn .field a,
.takeover-btn-wrap .tk-btn .field a:focus { 
color: $global_buttons_color;   
    border: 1px solid $global_buttons_border_color;
    box-shadow: 0px 1px 0px $global_buttons_boxshadow_color inset;
    -webkit-box-shadow: 0px 1px 0px $global_buttons_boxshadow_color inset;   
    $global_buttons_gradiant_bg  
}
.button_blue a:hover,
.button_blue a:focus,
#renewal-countdown-timer #edit-actions .form-submit:hover,
#renewal-countdown-timer #edit-actions .form-submit:focus,
.global-button:hover, 
.global-button:focus,
.cbtn button:hover,
.cbtn button:focus,
.global-button a:hover,
.global-button a:focus {
  color: $global_buttons_hover_color !important;
  background: $global_buttons_bg !important;
  border: 1px solid  $global_buttons_border_hover_color !important;
}
.global-button[disabled], .cbtn button[disabled] {
  border: 1px solid $buttons_global_border_color; 
}

/*------------------video-paly-icon Start------------------------*/
.vjs-default-skin .vjs-big-play-button { 
  background-color: $video_btn_bg_color; 
}
@media only screen and (max-device-width: 768px) {
  .vjs-default-skin .vjs-big-play-button { 
    background-color: $video_btn_bg_color;
  }
}
.vjs-default-skin:hover .vjs-big-play-button, .vjs-default-skin .vjs-big-play-button:focus {
  background-color: $video_btn_bg_hover_color; opacity: .75;
}

/*------------------video-paly-icon End------------------------*/
#modal_bootstrap .poupatstud { color: #ffffff; }
#modal_bootstrap .modal-dialog .modal-content {border: 1px solid #778087;}

/********************Content*******************/

.main-wrap .flex-control-paging li a.flex-active {
    background: $calender_home_game_bg; 
}
.main-wrap .flex-control-paging li a.flex-active:hover,
.main-wrap .flex-control-paging li a.flex-active:focus {
    background: $nav_drop_down_item_hover; 
    color: $nav_font_color;
}

/********************Header*******************/
.navbar-default {
    border-top: 2px solid $nav_top_border_color; 
    $nav_gradient
    -webkit-box-shadow: 0px 1px 0px $nav_top_background_shadow inset;
    box-shadow: 0px 1px 0px $nav_top_background_shadow inset; 
}

.navbar-default .nav>li>a:hover,
.navbar-default .nav>li>a:focus,
.navbar-default .navbar-nav>.active-trail>a.active-trail,
.navbar-default .nav li:hover>a {
    border: 1px solid $nav_item_hover_border; 
    $nav_gradient_hover
}
.navbar-default .navbar-nav li.open>a.dropdown-toggle {
    $nav_gradient_hover
    color: $nav_drop_font_color; 
}
.navbar-default .navbar-nav>.active>a,
.navbar-default .navbar-nav>.active>a:hover,
.navbar-default .navbar-nav>.active>a:focus {
    border: 1px solid $nav_item_hover_border; 
    $nav_gradient_hover
    color: $nav_font_color; 
}
.navbar-default .navbar-nav>li>a,
.navbar-default .navbar-nav>li>a:hover,
.navbar-default .navbar-nav>li>a:focus,
.navbar-default .navbar-nav .active>a,
.navbar-default .navbar-nav .active>a:hover,
.navbar-default .navbar-nav .active>a:focus {
    color: $nav_font_color; 
}
.navbar-default .dropdown-menu,
.navbar-default .open .dropdown-menu {
    background: $nav_drop_down_background;
    -webkit-box-shadow: inset 0px 0px 3px $nav_drop_down_background_shadow;
    box-shadow: inset 0px 0px 3px $nav_drop_down_background_shadow;
}
.navbar-default .dropdown-menu>li>a:hover,
.navbar-default .dropdown-menu>li>a:focus,
.navbar-default .open .dropdown-menu>li>a:hover,
.navbar-default .open .dropdown-menu>li>a:focus,
.navbar-default .dropdown-menu>li>a .active>a,
.navbar-default .open .dropdown-menu>li>a .active>a,
.navbar-default .dropdown-menu>li>a .active>a:hover,
.navbar-default .dropdown-menu>li>a .active>a:focus,
.navbar-default .open .dropdown-menu>li>a .active>a:hover,
.navbar-default .open .dropdown-menu>li>a .active>a:focus,
.navbar-default .dropdown-menu li.active a.active-trail.active,
.navbar-default .open .dropdown-menu li.active a.active-trail.active,
.navbar-default .dropdown-menu li.active a.active,
.navbar-default .open .dropdown-menu li.active a.active {
    background: $nav_drop_down_item_hover; 
    color: $nav_drop_down_item_font_color; 
    -webkit-box-shadow: inset 0px 0px 1px #000000;
    box-shadow: inset 0px 0px 1px #000000;
}
.event-gallary-thumb-slider .view-event-slider .view-content ul .views-row .tilehead,
.promo-tile .tilehead {    
   $primotile_and_title_head
}
.promo-tile .promo-box.regular a.promo-link:hover .second {    
    $primotile_hover
}
.calendar-legend .calendar-legend-right > li a.active, 
.calendar-legend .calendar-legend-right > li a:hover {
  border: 1px solid $calender_lr_border; 
  -webkit-box-shadow: 0px 1px 0px #dedede inset ;
  box-shadow: 0px 1px 0px #dedede inset ;
  text-shadow: -1px -1px 0 rgba(0, 0, 0, 0.3);
  background-image: $calender_lr_bg; 
}
.calendar-legend .nav > li > a:hover, .calendar-legend .nav > li > a:focus {
  /*Table bottom game and schedule event BG*/
  border: 1px solid $calender_lr_border; 
  -webkit-box-shadow: 0px 1px 0px #dedede inset ;
  box-shadow:  0px 1px 0px #dedede inset ;
  text-shadow: -1px -1px 0 rgba(0, 0, 0, 0.3);
  background-image: $calender_lr_bg;
}
.event-gallary-big-box .modal-header, #modal_bootstrap .modal-header,
#modal_bootstrap .modal-dialog.game-popup-dialog .modal-header {
 background: $calender_events_heading_bg;  
}
.pouptopwrap .sthevent-image .more-info,
.sthevent-body.more-info, .sthevent-form.openpop {
  background: $calender_home_game_squre !important; /*event-more-color*/
}
.calendar-legend .cal-home-legend:before {
  border: 1px solid $calender_home_border; 
  background: $calender_home_game_bg;
}
.calendar-legend .cal-away-legend:before {
    border: 1px solid $calender_home_border; 
    background: $calender_away_game_bg; 
}
.calendar-calendar .single-day .inner .item .game-home {
  background: $calender_home_game_squre !important; /*home-game-color*/
}
.calendar-calendar .single-day .inner .item .game-away {
  background: $calender_away_game_squre !important; /*away-game-color*/
}

.page-season-schedule .view-content .panel-group .panel-heading { $calender_listview_bg}
.page-season-schedule .view-content .panel-group .panel-heading:hover, 
.page-season-schedule .view-content .panel-group .next-game-active .panel-heading {$calender_listview_bg_hover}

#block-views-acct-reps-block {  background: $contact_us_bg_color;}

.container-fluid .page-header,
.container-fluid .page-header > div,
.view-header .date-nav-wrapper .date-heading h3 { color: $site_title_color; } /* page title text color*/
.promo-tile .tilehead span {color: $promo_title_color;} /* Promo title text color*/
.main-wrap .region-content .node, 
.main-wrap .region-content .node-video .field-name-field-bottom-content {
    $site_container_color    
  } /* Container background gradient color*/


 $addtional_css_overwrite

";



