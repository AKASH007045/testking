 <?php

/**
 * pts utility library
 */
class pts {
  private static $container;
  
  /**
   * @param ContainerAwareInterface $container
   *
   * @return void
   */
  public static function setContainer($container) {
    self::$container = $container;
  }

  /**
   * @return ContainerAwareInterface
   */
  private static function getContainer() {
    return self::$container;
  }

  /**
   * Encrypt credit card
   *
   * @param string $ccNumber         Credit card number
   * @param string $certificate_name Certificate name
   *
   * @return <array<string>> [$certVersion, $ccNumberEncrypted64]
   */
  private static function pts_encryptCC($ccNumber, $certificate_name = null) {
    $pts_conf = self::load_php('pts');

    $certificate_file_name = isset($pts_conf['certificate_path']) ? $pts_conf['certificate_path'] : null;
    if ($certificate_name === null) {
      $certificate_name = $pts_conf['latest_certificate_name'];
    }

    $certificate_file_name .= $certificate_name;
    $certificate_file_content = self::get_certificate_file_content($certificate_file_name);

    // Get Public Key
    $paySysPublicKey = openssl_pkey_get_public($certificate_file_content);
    if ($paySysPublicKey === false) {
      // Unable to open cert
      //pts_errorHandling(null, 'cert', null, "Unable to get ghe private key from cert file: $certFile");
      //Audit::display_audit(Audit::WARNING, "Unable to get the private key from cert file: $certificate_file_name", __FILE__, __LINE__);
      watchdog('Unable to get the private key from cert file', 'Unable to get the private key from cert file: %cert', array('%cert' => $certificate_file_name), WATCHDOG_WARNING);
      exit;
    }

    // Encrypt CC Number
    // Salt: First 16 of 32 chars returned by md5(rand())
    $ccNumberSalted = substr(md5(rand()), 0, 16) . $ccNumber;
    $ccEncrypted = '';
    if (openssl_public_encrypt($ccNumberSalted, $ccEncrypted, $paySysPublicKey, OPENSSL_PKCS1_PADDING) === false) {
      // Unable to Encrypt
      //pts_errorHandling(null, 'cert', null, "Unable to Encrypt CC");
      //Audit::display_audit(Audit::WARNING, "UNABLE to ENCRYPT credit card number", __FILE__, __LINE__);
      watchdog('Unable to encrypt cc number', 'UNABLE to ENCRYPT credit card number', WATCHDOG_WARNING);
      exit;
    }

    $ccNumberEncrypted64 = base64_encode($ccEncrypted);
    // $ccNumHash = sha1($ccNumber);
    $ccNumHash = self::TM_hash_credit_card($ccNumber);
    return array($certificate_name, $ccNumberEncrypted64, $ccNumHash, 'certificate_name' => $certificate_name, 'ccNumberEncrypted64' => $ccNumberEncrypted64, 'cc_num_hash' => $ccNumHash);
  }

  private static function TM_hash_credit_card($credit_card_number)
  {
      $salt = 'MsPiD';

     if (1 & ord(substr($credit_card_number, -1))) {
          $credit_card_number = $credit_card_number . $salt;
      } else {
          $credit_card_number = $salt . $credit_card_number;
      }

     return strtoupper(sha1($credit_card_number));
  }
  /**
   * Get certificate content
   *
   * @param string $certificate_file_name File containing the certificate.
   *
   * @return string
   */
  private static function get_certificate_file_content($certificate_file_name) {
    $certificate_file_content = file_get_contents($certificate_file_name);
    if (strlen($certificate_file_content) === 0) {
      // Empty file or unable to read file
      //pts_errorHandling(null, 'cert', null, "Empty file or unable to read cert file: $certFile");
      //Audit::display_audit(Audit::WARNING, "Empty file or unable to read cert file: $certificate_file_name", __FILE__, __LINE__);
      watchdog('Empty cert file', 'Empty file or unable to read cert file: %cert', array('%cert' => $certificate_file_name), WATCHDOG_WARNING);
      exit;
    }
    return $certificate_file_content;
  }

  /**
   * Encrypt credit card number
   *
   * @param string $credit_card_number Credit card number
   *
   * @return <array<string>>[,]
   */
  public static function encrypt_credit_card_number($credit_card_number, $certificate_name) {
    $encrypted_credit_card_number = self::pts_encryptCC($credit_card_number, $certificate_name);
    return $encrypted_credit_card_number;
  }

  /**
   * Loads php config file
   *
   * @param string $filename PHP config file name
   *
   * @return array
   */
  public static function load_php($filename) {
    $config = array();
    if (self::getContainer() !== null) {
      if (self::getContainer()->hasParameter(RegistryKey::CONF_CONFIGURATION)) {
        $config = Arr::get(self::getContainer()->getParameter(RegistryKey::CONF_CONFIGURATION)['services'], $filename, array());
      }
      else {        
        //Log::instance()->debug('else reached for ' . $filename . '" ' . __METHOD__);
        watchdog('reached for', 'else reached for :file having :method', array(':file' => $filename, ':method' => __METHOD__), WATCHDOG_NOTICE);
      }
    }
    else {
      //Log::instance()->debug('container is empty ' . $filename . '" ' . __METHOD__);
      watchdog('empty container', 'container is empty in :file having :method', array(':file' => $filename, ':method' => __METHOD__), WATCHDOG_NOTICE);
    }
    return $config;
  }
}
