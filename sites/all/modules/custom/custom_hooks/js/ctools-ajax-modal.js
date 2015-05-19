/**
* Provide the HTML to create the modal dialog.
*/
Drupal.theme.prototype.CToolsGneshareModal = function () {
  var html = '';

  html += '<div id="ctools-modal" class="popups-box popUpPanelContainer">';
  html += '  <div class="ctools-modal-content mcco-share-popup popUpPanel clearfix">';
  html += '          <div class="popups-container">';
  html += '            <div class="modal-header popups-title">';
  html += '                <div class="popUpPanelHeader">';
  $html += '                 <img src="images/starSmall.png" alt="" title="" class="img1" />';
  $html += '                 <img src="images/closeIcon.png" alt="" title="" class="closeIcon" />';
  $html += '              </div>';
  html += '              <span class="popups-close"><a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
  html += '            </div>';
  html += '            <div id="modal-content" class="modal-content popups-body"></div>';
  html += '          </div>';
  html += '  </div>';
  html += '</div>';

  return html;
}

