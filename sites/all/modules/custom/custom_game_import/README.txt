
-- SUMMARY --

The Custom game import module is a helper module for nba/nhl/nfl module. To add any other module please add inside the modules folder
eg. /custom_game_import/modules/

-- REQUIREMENTS --
1. Need team set up and their should be only one team as a home team


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* You need to install this module and its related child module to work imporing feeds successfully.


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions:
  
  - Administer Ticketmaster Data Import
    Users in roles with the "Administer Ticketmaster Data Import" permission will able to import the feed
    
* Feed import settings form. We have to add the nba feed url
  - admin/manage/settings/nba_feed_import_settings *

    * if you add more than one module then several config settings may be used.
  

-- CUSTOMIZATION --

* You need to use $home_team_id = custom_nba_get_hometeam_nid() in custom_hook module for get_team_info()


