<?php $arg = arg(1); ?>
<div class="home-tgl"><div class="wrapper-icon-sidekick"><i class="fa fa-angle-right"></i><span class="sidekick-label"></span></div></div>
<div class="tabs-wrap-primary-user"><div class="tabsoption-primary-user"> 

<ul class="page-name"><li>Page:</li> <li>Home</li></ul>
<div class="view-option-user"><label>User Type:</label> <select name="homeselect" class="form-control" id="homeselect" onchange="this.options[this.selectedIndex].value && (window.location = this.options[this.selectedIndex].value);">
      <option <?php print (isset($arg) && $arg ==1) ? 'selected="selected"' : '';?> value="/home/1">Public</a></option>
      <option <?php print ($arg ==0 || drupal_is_front_page()) ? 'selected="selected"' : '';?> value="/home/0">Logged In</a></option>
    </select> </div></div>
 <ul class="tabs-primary-live nav">
     <li class="live">Live View</li>
    <li class="edit"><a href="/"><i class="fa fa-edit"></i> Edit</a></li>
</ul>
</div>
