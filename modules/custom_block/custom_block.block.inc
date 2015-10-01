<?php

// @file

/**
 * function which will return account representative block
 */
function custom_block_account_rep($block_id = 'block') {
  global $user;
  $acct_id = $user->name;
  if (isset($acct_id)) {
    $acct_manager = acct_rep_load($acct_id);
    if (isset($acct_manager->rep_acct_id) && $acct_manager->rep_acct_id) {
      return views_embed_view('acct_reps', $block_id, $acct_manager->rep_acct_id);
    }
    else {
      return views_embed_view('acct_reps', $block_id, 1);
    }
  }
  return '';
}

/**
 * Welcome user block
 */
function custom_block_user_welcome_block() {
  global $user;
  if ($user->uid) {
    $acct_id = $user->name;
    if ($acct_id) {
      $user_info = get_user_info($acct_id);
      if (isset($user_info->acct_id)) {
        if ($user_info->name_first) {
          $name = $user_info->name_first . ' ' . $user_info->name_last ;
        }
        elseif ($user_info->name2_first) {
          $name = $user_info->name2_first . ' ' . $user_info->name2_last ;
        }
        elseif ($user_info->owner_name) {
          $name = $user_info->owner_name;
        }
        elseif ($user_info->company_name) {
          $name = $user_info->company_name ;
        }
        elseif ($user_info->name2_company) {
          $name = $user_info->name2_company ;
        }
        $date = $user_info->add_date ? custom_modified_date($user_info->add_date) : 0;
        $name = '<span class="user-name">' . $name . '</span>';
        //$name .= isset($date) ? ' EST ' . $date : '';
        return $name;
      }
    }
  }
  return '';
}
/**
 * Home page banner welcome user message
 */
function custom_block_homepage_welcome_message() {
  global $user;
  $btn = '';
  if ($user->uid) {
    $acct_id = $user->name;
    if ($acct_id) {
      $name = get_user_full_name($user);
      $get_amount_info = get_tm_user_amount_info($acct_id);
      if (isset($get_amount_info['due_amt']) && $get_amount_info['due_amt'] > 0) {
        //$btn = '<a class="home-pay-now" href="/invoice">PAY NOW</a>';
        $message =  t(variable_get('welcome_sth', '<span>WELCOME !user,</span><br/><span class="home-subs-small-head">YOUR INVOICE IS READY</span><span>ARE YOU?</span><span class="home-pay-now" href="/invoice">PAY NOW</span>'), array('!user' => $name)) . $btn;
      }
      else {
        $message = t(variable_get('welcome_sth_paid', '<span>WELCOME !user,</span><br/><span class="home-subs-small-head">THE 2014 SEASON</span><span>STARTS NOW</span>'), array('!user' => $name));
      }
    }
  }
  else {
    $message = variable_get('welcome_anon', t('<span>LOG IN TO BECOME A PART OF</span><br/><span class="home-subs-head">HISTORY</span><span>BEFORE IT\'S TOO LATE</span>'));
  }
  // return the welcome message for anonymous users
  return $message;
}

function custom_modified_date($date) {
  $time = strtotime($date);
  return date('Y', $time);
}

function custom_block_promo_tiles() {
  global $user;
  $query = db_select('node', 'n');
  $query->leftJoin('weight_weights', 'w', 'w.entity_id = n.nid');
  $query->fields('n', array('nid'))
  ->condition('status', 1, '=')
  ->condition('type', 'promo_tile', '=')
  ->orderBy('weight', 'ASC');
  $nodes = $query->execute()->fetchAll();
  $count_anon = 0;
  $count_loggedin = 0;
  $promo = '';
  $promo_anonymous= '';
  $promo_logged_in = '';
  foreach($nodes as $nid):
    $node = node_load($nid->nid);
    $lang = isset($node->language) ? $node->language : LANGUAGE_DEFAULT;
    $user_type = (isset($node->field_user_type[$lang][0]['value'])) ? $node->field_user_type[$lang][0]['value'] : '';
    if ($user_type == 1 && $count_loggedin < 3) {
      $promo_logged_in .= render(node_view($node, $view_mode = 'full', $langcode = NULL));
      $count_loggedin++;
    }
    if($user_type == 0 && $count_anon < 3) {
      $promo_anonymous .= render(node_view($node, $view_mode = 'full', $langcode = NULL));
      $count_anon++;
    }
    if ($count_loggedin>=3 && $count_anon >=3) {
      break;
    }
  endforeach;
  if (user_access('Editor') && (arg(0) == 'home') && arg(1) == 1) {
    return $promo_anonymous;
  }
  if ($user->uid) {
    return $promo_logged_in;
  }
  return $promo_anonymous;
}

function custom_block_main_navigation() {
  // Check for active menu items to render menu block
  global $user;
  $block = module_invoke('menu_block', 'block_view', '3');
  $block2 = module_invoke('menu_block', 'block_view', '6');
  $block3 = module_invoke('menu_block', 'block_view', '4');
  if ($count = count($block['content']) && $user->uid && (isset($block['content']['#config']['menu_name']) && $block['content']['#config']['menu_name']!= 'user-menu')) {
    return render($block['content']);
  }
  if ($user->uid) {
    return render($block2['content']); 
  }
  else {
    return render($block3['content']); 
  }
}