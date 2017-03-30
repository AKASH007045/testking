<?php

$data = array(
  'bundles' => array(
    'stp_thankyou_page' => (object) array(
      'type' => 'stp_thankyou_page',
      'name' => 'STP Thankyou Page',
      'base' => 'node_content',
      'module' => 'node',
      'description' => 'Showing thankyou message on payment completion.',
      'help' => '',
      'has_title' => '1',
      'title_label' => 'Title',
      'custom' => '1',
      'modified' => '1',
      'locked' => '0',
      'disabled' => '0',
      'orig_type' => 'stp_thankyou_page',
      'disabled_changed' => FALSE,
      'bc_entity_type' => 'node',
    ),
  ),
  'fields' => array(
    'body' => array(
      'entity_types' => array(
        0 => 'node',
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_body' => array(
                'value' => 'body_value',
                'summary' => 'body_summary',
                'format' => 'body_format',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_body' => array(
                'value' => 'body_value',
                'summary' => 'body_summary',
                'format' => 'body_format',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(
        'format' => array(
          'table' => 'filter_format',
          'columns' => array(
            'format' => 'format',
          ),
        ),
      ),
      'field_name' => 'body',
      'type' => 'text_with_summary',
      'module' => 'text',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
        ),
        'summary' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
        ),
        'format' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
        ),
      ),
      'bundles' => array(
        'node' => array(
          0 => 'page',
          1 => 'article',
          2 => 'webform',
          3 => 'acct_rep',
          4 => 'stp_page',
          5 => 'take_over_pages',
          6 => 'seat_info',
          7 => 'typeform',
          8 => 'event',
          9 => 'email_template',
          10 => 'stp_thankyou_page',
          11 => 'new_home_page',
        ),
      ),
    ),
    'field_plan_segment_account_group' => array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(
        'allowed_values' => array(
          1 => '1',
          33 => '33',
          40 => '40',
          34 => '34',
          35 => '35',
          36 => '36',
        ),
        'allowed_values_function' => '',
      ),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_field_plan_segment_account_group' => array(
                'value' => 'field_plan_segment_account_group_value',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_field_plan_segment_account_group' => array(
                'value' => 'field_plan_segment_account_group_value',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'field_name' => 'field_plan_segment_account_group',
      'type' => 'list_text',
      'module' => 'list',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array(
        'value' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
        ),
      ),
      'bundles' => array(
        'node' => array(
          0 => 'take_over_pages',
          1 => 'stp_thankyou_page',
        ),
      ),
    ),
    'field_template_type' => array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(
        'allowed_values' => array(
          'invoice' => 'Invoice',
          'buy' => 'Buy',
          'email-invoice' => 'Email Invoice',
          'upgrade' => 'Upgrade',
        ),
        'allowed_values_function' => '',
      ),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_field_template_type' => array(
                'value' => 'field_template_type_value',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_field_template_type' => array(
                'value' => 'field_template_type_value',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'field_name' => 'field_template_type',
      'type' => 'list_text',
      'module' => 'list',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array(
        'value' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
        ),
      ),
      'bundles' => array(
        'node' => array(
          0 => 'email_template',
          1 => 'stp_thankyou_page',
        ),
      ),
    ),
  ),
  'instances' => array(
    'body' => array(
      0 => array(
        'label' => 'Message',
        'widget' => array(
          'weight' => '3',
          'type' => 'text_textarea_with_summary',
          'module' => 'text',
          'active' => 1,
          'settings' => array(
            'rows' => '20',
            'maxlength_js' => '',
            'maxlength_js_enforce' => 0,
            'maxlength_js_truncate_html' => 0,
            'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
            'summary_rows' => 5,
          ),
        ),
        'settings' => array(
          'text_processing' => '1',
          'display_summary' => 1,
          'better_formats' => array(
            'allowed_formats_toggle' => 0,
            'allowed_formats' => array(
              'full_html' => 'full_html',
              'test_paging' => 'test_paging',
              'filtered_html' => 'filtered_html',
              'plain_text' => 'plain_text',
              'typeform_html' => 'typeform_html',
              'event_more_info' => 'event_more_info',
            ),
            'default_order_toggle' => 0,
            'default_order_wrapper' => array(
              'formats' => array(
                'full_html' => array(
                  'weight' => '-10',
                ),
                'test_paging' => array(
                  'weight' => '-9',
                ),
                'filtered_html' => array(
                  'weight' => '-8',
                ),
                'plain_text' => array(
                  'weight' => '-7',
                ),
                'typeform_html' => array(
                  'weight' => '0',
                ),
                'event_more_info' => array(
                  'weight' => '0',
                ),
              ),
            ),
          ),
          'user_register_form' => FALSE,
        ),
        'display' => array(
          'default' => array(
            'label' => 'hidden',
            'type' => 'text_default',
            'settings' => array(
              'quickedit' => array(
                'editor' => 'plain_text',
              ),
            ),
            'module' => 'text',
            'weight' => 0,
          ),
          'teaser' => array(
            'label' => 'hidden',
            'type' => 'text_summary_or_trimmed',
            'settings' => array(
              'trim_length' => 600,
              'quickedit' => array(
                'editor' => 'form',
              ),
            ),
            'module' => 'text',
            'weight' => 0,
          ),
          'acct_rep_detail' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_right' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_box' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'home_page_tout' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'countdown_campaign_tile' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'next_game' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_calendar' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_short' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_node' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
        ),
        'required' => 1,
        'description' => '',
        'default_value' => NULL,
        'field_name' => 'body',
        'entity_type' => 'node',
        'bundle' => 'stp_thankyou_page',
        'deleted' => '0',
      ),
    ),
    'field_plan_segment_account_group' => array(
      0 => array(
        'label' => 'Account Group ID',
        'widget' => array(
          'weight' => '1',
          'type' => 'options_select',
          'module' => 'options',
          'active' => 1,
          'settings' => array(
            'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
          ),
        ),
        'settings' => array(
          'user_register_form' => FALSE,
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'type' => 'list_default',
            'settings' => array(
              'quickedit' => array(
                'editor' => 'form',
              ),
            ),
            'module' => 'list',
            'weight' => 2,
          ),
          'teaser' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_detail' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_right' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_box' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'home_page_tout' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'countdown_campaign_tile' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'next_game' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_calendar' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_short' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_node' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
        ),
        'required' => 1,
        'description' => '',
        'default_value' => NULL,
        'field_name' => 'field_plan_segment_account_group',
        'entity_type' => 'node',
        'bundle' => 'stp_thankyou_page',
        'deleted' => '0',
      ),
    ),
    'field_template_type' => array(
      0 => array(
        'label' => 'Type',
        'widget' => array(
          'weight' => '2',
          'type' => 'options_select',
          'module' => 'options',
          'active' => 1,
          'settings' => array(
            'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
          ),
        ),
        'settings' => array(
          'user_register_form' => FALSE,
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'type' => 'list_default',
            'settings' => array(
              'quickedit' => array(
                'editor' => 'form',
              ),
            ),
            'module' => 'list',
            'weight' => 3,
          ),
          'teaser' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_detail' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_right' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'acct_rep_box' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'home_page_tout' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'countdown_campaign_tile' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'next_game' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_calendar' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_short' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
          'event_node' => array(
            'type' => 'hidden',
            'label' => 'above',
            'settings' => array(),
            'weight' => 0,
          ),
        ),
        'required' => 1,
        'description' => '',
        'default_value' => NULL,
        'field_name' => 'field_template_type',
        'entity_type' => 'node',
        'bundle' => 'stp_thankyou_page',
        'deleted' => '0',
      ),
    ),
  ),
);