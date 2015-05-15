<?php $arg = arg(1); ?>
<div class="tabs-wrap-primary-user"><div class="tabsoption-primary-user"> 
<div class="home-tgl"><i class="fa fa-angle-right"></i></div>
<ul class="page-name"><li>Page:</li> <li>Home</li></ul>
<div class="view-option-user"><label>User Type:</label> <select name="homeselect" id="homeselect" onchange="this.options[this.selectedIndex].value && (window.location = this.options[this.selectedIndex].value);">
      <option <?php print (isset($arg) && $arg ==1) ? 'selected="selected"' : '';?> value="/home/1">Anonymous</a></option>
      <option <?php print ($arg ==0 || drupal_is_front_page()) ? 'selected="selected"' : '';?> value="/home/0">Logged In</a></option>
    </select> </div></div>
<!--<ul class="tabs--primary nav nav-tabs">
    <li class="live-draft"> Live Draft</li>
    <li class="live-view"><a class="active" href="#">View</a></li>
    <li class="live-edit"><a href="#">Edit</a></li>
</ul>-->
 <ul class="tabs-primary-live nav nav-tabs">
      <li class="live"> Live</li>
    <!--<li class="view"><a class="active" href="/"><i class="fa fa-eye"></i> View</a></li>-->
    <li class="edit"><a href="/"><i class="fa fa-edit"></i> Edit</a></li>
</ul>
<!-- <button class="view-versions-btn">View Versions</button>
 <button class="publish-btn">Publish</button>-->
</div>
