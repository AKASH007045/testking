jQuery(document).ready(function(){
    var host_url = window.location.href;
    var user_name = Drupal.settings.custom_hooks.custom_name;

    function set_active(main_nav_link, sub_nav_text){
        jQuery("ul.menu > li > a").each(function(){
            //This condition will filter out My home game and events and Full schedule
            // from calendar page
            if(!jQuery(this).parents("ul.menu").hasClass("calendar-legend-right")){
                var attrstr = jQuery(this).attr('href');

                if(attrstr.indexOf(main_nav_link)>-1){
                    jQuery(this).addClass('active-trail active').parent().addClass('active-trail active');
                    var context = jQuery(this).parent();

                    if(sub_nav_text){
                        sub_nav_text = sub_nav_text.toLowerCase();
                        jQuery("ul.dropdown-menu > li > a", context).each(function(){
                            var subattr = jQuery(this).text();
                            subattr = subattr.toLowerCase();
                            if(subattr==sub_nav_text){
                                jQuery(this).addClass('active-trail active').parent().addClass('active-trail active');
                            }
                        });
                    }
                }
            }
        });
    }


    //TODO
    //Replace with dynamic values
    if (navigator.userAgent.indexOf('Safari') != -1 && navigator.userAgent.indexOf('Chrome') == -1) {
        jQuery('div.row-header ul.menu > li > a').mouseover(function(){
            jQuery(this)
                .parents("li").addClass("active open");
        }).mouseout(function(){
            var isHovered = (jQuery(this).parents("li").find('ul.dropdown-menu:hover').length > 0) ? 1 : 0;
            if(!isHovered){
                if(!jQuery(this).parents("li").hasClass("active-trail")){
                    jQuery(this)
                        .blur()
                        .parents("li").removeClass("active open");
                }
            }
        });
    }
    jQuery('div.row-header ul.menu ul.dropdown-menu').prev("a").mouseover(function(){
        jQuery(this)
            .parents("li").addClass("active open");

        jQuery(this).parents("li").find('ul.dropdown-menu').show();
    }).mouseout(function(){
        var isHovered = (jQuery(this).parents("li").find('ul.dropdown-menu:hover').length > 0) ? 1 : 0;
        if(!isHovered){
            if(!jQuery(this).parents("li").hasClass("active-trail")){
                jQuery(this)
                    .blur()
                    .parents("li").removeClass("active open");
            }
            jQuery(this).parents("li").find('ul.dropdown-menu').hide();
        }
    });

    jQuery('div.row-header ul.menu ul.dropdown-menu').mouseover(function(){
        jQuery(this).parents("li").find('ul.dropdown-menu').show();
        jQuery(this).parents("li").find("a.dropdown-toggle")
            .parents("li").addClass("active open");
    }).mouseout(function(){
        if(!jQuery(this).parents("li").hasClass("active-trail")){
            jQuery(this).parents("li").find("a.dropdown-toggle")
                .parents("li").removeClass("active open");
        }
        jQuery(this).parents("li").find('ul.dropdown-menu').hide();
    });

    //Highlight menu item for events page and calendar page
    if(host_url.indexOf("full-calendar") > -1){
        set_active('/my-calendar', 'calendar');
    }
    else if(host_url.indexOf("my-calendar") > -1){
        set_active('/my-calendar', 'calendar');
    }
    else if(host_url.indexOf("season-schedule") > -1){
        set_active('/my-calendar', 'calendar');
    }
    else if(host_url.indexOf("schedule") > -1){
        set_active('/calendar', 'calendar');
    }
    else if(host_url.indexOf("/event/") > -1){
        set_active('/my-calendar', 'STH Events');
    }
    else{
        jQuery("ul.menu > li.active-trail").addClass('active');
    }
});