<?php

$css = "
/*
RecordID = $mid;
User: $uid;
Time: $timestamp;
*/

/*----------brandColor------------*/

.siteColor {                                          /*----Site color-----*/
    color: $site_color;
}

a {
  color: $site_color;
}
.navIcon span {
  background-color: $site_color;
}
.bar-header .button {                                           /*----hambuger icon-----*/
    color: $top_header_button_bg_color;
}
.bar-header {                                                   /*----header background-----*/
   background-color: $header_bg_color;
   background-image: url($mob_header_top_bg) ;
                                                              /*----header border bottom-----*/
    border-bottom:1px solid $header_border_bottom_color;
                                                                /*----header border shadow -----*/
    box-shadow: 0 0 10px #000000;
}
.bar-header.has-tabs-top {
  background-image: url($mob_header_top_bg) !important;
}
.button,
.NeehelpContent .button_blue a {                                                       /*----Global button background-color-----*/
    $default_button_gradient
}
.button,
.NeehelpContent .button_blue a {                                                       /*----Global button text color-----*/
    color: $mob_global_btn_color;
}

.button-black-border {                                          /*----Global button-black-border-----*/
    border: 1px solid $mob_global_btn_blk_border_color;
}

.button.active, .button.activated,
.having-trouble-box .button_blue > a.active, 
.having-trouble-box .button_blue > a.activated {                             /*----Global button activaed color-----*/
    background: $mob_global_btn_active_color;
}
.slider-pager .slider-pager-page {                              /*----slider-pager-page-----*/
    color: $mob_pager_circle; 
}
.sidebar {                                                      /*----slider-background-color-----*/
    background-color: $side_bar_bg; /*#562714;*/
    border-right: 1px solid $side_bar_bg_border_rt; /*#787878;*/
}

.sidebar .item.activated .menuItemLink, 
.sidebar .item.active .menuItemLink,
.sidebar .submenu .item.activated {                            /*----slider-menu-active and hover-----*/
    $nav_gradient
    border-color: rgba(0, 0, 0, 0.5);
}

.sidebar .subitem.activated .menuItemLink, 
.sidebar .subitem.active .menuItemLink {                        /*----slider-menu-color-----*/
    background: rgba(0, 0, 0, 0.5);
}
.invoiceColor,
.contactPerson .contactPersonInfo .contactPerson-Details .contactEmail a {                                                 /*--invoiceColor--*/
  color: $mob_invoice_btn_color;
}
.invoiceDetails .tabs .tab-item.active span {                   /*--invoice bottom tab active color--*/
background-color: $mob_invoice_btn_color;
border: 1px solid $mob_global_btn_blk_border_color;}
.calenderViewLink .calViews .calList.activated,
.calenderViewLink .calViews .calList.active,
.calenderViewLink .calViews .calMonth.activated, 
.calenderViewLink .calViews .calMonth.active {                   /*----Calendar month and list view-----*/
  /*color: $site_color;*/ 
   color: $mob_invoice_btn_color;
}
#gCalendar .fc-view-container .fc-head th {
                                                                /*----Calendar table header -----*/
$cal_table_header_color
color:#000000;
}

#gCalendar .fc-day-grid {background-image: url($stp_calendar_mob_bg) ;}

.sthEventsScreen .sthEventsContainer .sthEventList .sthEventListItem .sthEventsHeader .sthEventsName { /*----Calendar event header -----*/
    $cal_sth_event_bg_gradiant
}
.sthEventsScreen .sthEventsContainer .sthEventList .sthEventListItem .sthEventsHeader .sthEventsLogo,
.sthEventsScreen .sthEventsContainer .sthEventList .sthEventListItem .sthEventsHeader .sthEventsLogo span { /*----Calendar event header logo background color -----*/
    background: $cal_sth_event_logobg;
}

.sthEventsScreen .sthEventsContainer .sthEventList .sthEventListItem .sthEventsHeader {  /*----Calendar event header top border gradient color -----*/
$cal_sth_event_top_bg_gradiant
  
}
.sidebar .item {                                                                                   /*----sidebar menu-----*/
    color: $menu_item_text_color;                                                                 /*----sidebar menu bottom border-----*/
    border-color: $menu_items_border;
}

.sidebar .item .menuItemLink {
background-color: $menu_item_bg_color;
}

.sidebar .sidebar-logo{ 
    background-color: $header_bg_color;                                                                        /*----sidebar menu top logo border-bottom -----*/
    border-bottom: 1px solid $side_bar_bg_border_bottom;
}
.sidebar .sidebar-logo span {

$side_bar_stp_logo_bg;
}

.calendar-legend p span.cal-legend .cl-home {  
    background-color:$home_game_legend;}

#gCalendar .calanderThumbImage.calender-home-event {
   background-color: $home_game_color; } 

.calendar-legend p span.cal-legend .cl-away-legend { 
    background-color:$away_game_legend; }

#gCalendar .calanderThumbImage.calender-away-event {
  background-color:$away_game_color; 
}

.fc-unthemed .fc-today {background-color:$cal_today_bg_color; }

.spinner-ios, .spinner-ios-small, .spinner {stroke:$site_color !important;}

$addtional_css_overwrite

@media (orientation: portrait) {
    .splashImg {
        background-image: url($mob_splash_img_home) ;
    } 
    }
    @media (orientation: landscape) {
.splashImg {
        background-image:  url($mob_splash_img_landscape) ;
    }

    }

}";
