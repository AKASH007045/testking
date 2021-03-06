<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
 
?>
<div class="container-fluid">
<div class="heroHolder clearfix">
        
        <article  id="billboard">
          <section class="col-lg-12 col-md-12 col-sm-12 hero">
          
          <!--<div class="hero bg-holder">
              <img src="http://cow.iomedia.dev/sites/all/themes/sports/images/main-bg.jpg" alt="" title="" />
          </div>-->
          
		  <header>
		 <div class="row row-header">
		 <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 username">
		   <?php if (!empty($page['user_name'])): ?>
			 <?php print render($page['user_name']); ?>
		   <?php endif; ?>
		 </div>
		 <div class="col-lg-12 col-md-12 mainNavigation"><!--col-sm-12 col-xs-12-->
		  <div class="navbar navbar-default" role="navigation">
		   <div class="navbar-header">
	<button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse"> <span class="sr-only">Toggle navigation</span> <span class="icon-bar"></span> <span class="icon-bar"></span> <span class="icon-bar"></span> </button>
				<a class="navbar-brand" href="/" title="<?php print $site_name; ?>"><img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" class="img-responsive"/></a>
				</div>
			  <div class="navbar-collapse collapse">
				<?php if (!empty($page['main_menu'])): ?>
			         <?php print render($page['main_menu']); ?>
		                <?php endif; ?>
				<ul class="nav navbar-nav navbar-right">
				  <?php if (!empty($page['user_info'])): ?>
					<?php print render($page['user_info']); ?>
				  <?php endif; ?>
				</ul>
			  </div>
			  <!--/.nav-collapse --> 
			<!--/.container-fluid --> 
		  </div>
		  </div>
		  </div>
		</header>
		
		<?php if (!empty($page['content'])): ?>
		 <section class="container"> 
		 <article class="jumbotron">
		    <?php print render($page['content']); ?>
		</article>
		</section>
	  <?php endif;?>
		  
			  <!-- Next Game -->
				<?php if (!empty($page['game_next'])): ?>
				<section class="col-lg-12 col-md-12 col-sm-12 home-nextgamebg clearfix"><!--col-xs-12-->
				  <div class="container">
					 <aside class="nextgamevs">
					  <?php print render($page['game_next']);?>
					  </aside>
				  </div>
				</section>
				<?php endif;?>
				<!-- / Next Game --> 
		  
		  </section>
		  </article>
		</div>
		  
  <!-- Static navbar -->
 
  <!-- Main component for a primary marketing message or call to action -->
   
  <section class="container main-wrap">
    <?php if (!empty($page['highlighted'])): ?>
      <div class="highlighted"><?php print render($page['highlighted']); ?></div>
    <?php endif; ?>
    <a id="main-content"></a>
    <?php if (!empty($tabs)): ?>
      <?php print render($tabs); ?>
    <?php endif; ?>
    <?php if (!empty($action_links)): ?>
      <ul class="action-links"><?php print render($action_links); ?></ul>
    <?php endif; ?>    
</section>
<!-- /container --> 



<!-- Footer tile -->
<section class="col-lg-12 col-md-12 col-sm-12 col-xs-12 promo-tile clearfix">
  <div class="row">
    <?php if (!empty($page['social'])): ?><div class="block-box-ssl col-lg-3 col-md-3 col-sm-3 col-xs-6 "><?php  print render($page['social']); ?></div><?php endif; ?>
    <?php  print render($page['promo_tile']); ?>
  </div>
</section>
<footer class="col-lg-12 col-md-12 col-sm-12 col-xs-12 footer-copyright">
<div class="row clearfix">
<!-- Footer tile -->    
     <?php print render($page['footer']); ?>
  </div>
</footer>
<?php print $background_update_link;?>
</div>