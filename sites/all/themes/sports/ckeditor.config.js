/*
Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/*
 WARNING: clear browser's cache after you modify this file.
 If you don't do this, you may notice that browser is ignoring all your changes.
 */
CKEDITOR.editorConfig = function(config) {
  config.indentClasses = [ 'rteindent1', 'rteindent2', 'rteindent3', 'rteindent4' ];

  // [ Left, Center, Right, Justified ]
  config.justifyClasses = [ 'rteleft', 'rtecenter', 'rteright', 'rtejustify' ];

  // The minimum editor width, in pixels, when resizing it with the resize handle.
  config.resize_minWidth = 450;

  // Protect PHP code tags (<?...?>) so CKEditor will not break them when
  // switching from Source to WYSIWYG.
  // Uncommenting this line doesn't mean the user will not be able to type PHP
  // code in the source. This kind of prevention must be done in the server
  // side
  // (as does Drupal), so just leave this line as is.
  config.protectedSource.push(/<\?[\s\S]*?\?>/g); // PHP Code

  // [#1762328] Uncomment the line below to protect <code> tags in CKEditor (hide them in wysiwyg mode).
  // config.protectedSource.push(/<code>[\s\S]*?<\/code>/gi);
  config.extraPlugins = '';

  /*
    * Append here extra CSS rules that should be applied into the editing area.
    * Example:
    * config.extraCss = 'body {color:#FF0000;}';
    */
  config.extraCss = '';
  /**
    * Sample extraCss code for the "marinelli" theme.
    */
  if (Drupal.settings.ckeditor.theme == "marinelli") {
    config.extraCss += "body{background:#FFF;text-align:left;font-size:0.8em;}";
    config.extraCss += "#primary ol, #primary ul{margin:10px 0 10px 25px;}";
  }
  if (Drupal.settings.ckeditor.theme == "newsflash") {
    config.extraCss = "body{min-width:400px}";
  }

  /**
    * CKEditor's editing area body ID & class.
    * See http://drupal.ckeditor.com/tricks
    * This setting can be used if CKEditor does not work well with your theme by default.
    */
  config.bodyClass = '';
  config.bodyId = '';
  /**
    * Sample bodyClass and BodyId for the "marinelli" theme.
    */
  if (Drupal.settings.ckeditor.theme == "marinelli") {
    config.bodyClass = 'singlepage';
    config.bodyId = 'primary';
  }
  config.extraPlugins = 'lineheight';
  config.line_height="100%/1;125%/1.25;150%/1.5;175%/1.75;200%/2;225%/2.25;250%/2.5;275%/2.75;300%/3;325%/3.25;350%/3.5;375%/3.75;400%/4;425%/4.25;450%/4.5;475%/4.75;500%/5";

}


/*
 * Sample toolbars
 */

//Toolbar definition for basic buttons
Drupal.settings.cke_toolbar_DrupalBasic = [ [ 'Format', 'Bold', 'Italic', '-', 'NumberedList','BulletedList', '-', 'Link', 'Unlink', 'Image' ] ];

//Toolbar definition for Advanced buttons
Drupal.settings.cke_toolbar_DrupalAdvanced = [
  ['Styles', 'Font','FontSize'],
  ['Bold','Italic','Underline','RemoveFormat'],
  ['TextColor', 'BgColor'],
  ['NumberedList','BulletedList','-','Outdent','Indent'],
  ['JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock'],
  '/',
  ['Undo','Redo'],
  ['Scayt'],
  ['Link','Unlink'],
  ['Image','Table','HorizontalRule','SpecialChar'],
  ['PageBreak','Templates'],['Source']
];

// Toolbar definition for all buttons
Drupal.settings.cke_toolbar_DrupalFull =
[
  ['Styles', 'Font','FontSize'],
  ['Bold','Italic','Underline','RemoveFormat'],
  ['TextColor', 'BgColor'],
  ['NumberedList','BulletedList','-','Outdent','Indent'],
  ['JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock'],
  '/',
  ['Undo','Redo'],
  ['Scayt'],
  ['Link','Unlink'],
  ['Image','Table','HorizontalRule','SpecialChar'],
  ['PageBreak','Templates'],['Source']
];
/*[
  ['Source'],
  ['Cut','Copy','Paste','PasteText'],
  ['Undo','Redo','Find','Replace'],
  ['Styles', 'Font','FontSize', 'lineheight'],
  ['Bold','Italic','Underline','Strike','-','Subscript','Superscript','-','RemoveFormat'],
  ['NumberedList','BulletedList','-','Outdent','Indent'],
  ['JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock'],
  ['Link','Unlink'],
  ['Image','Table','HorizontalRule','SpecialChar'],
  ['PageBreak','Templates']
];*/
