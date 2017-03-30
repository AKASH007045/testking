<div class="stp-invoice-list-page">
    <ul class="nav nav-tabs" role="tablist">
       <li role="presentation" class="active"><a href="#pending" aria-controls="home" role="tab" data-toggle="tab" aria-expanded="true">Current Invoices</a></li>
       <li role="presentation" class=""><a href="#past" aria-controls="profile" role="tab" data-toggle="tab" aria-expanded="false">Paid Invoices</a></li>
     </ul>

    <!-- Tab panes -->
    <div class="tab-content">
      <div role="tabpanel" class="tab-pane active" id="pending"><?php print $data['pending'];?></div>
      <div role="tabpanel" class="tab-pane" id="past"><?php print $data['past'];?></div>
    </div>
 </div>