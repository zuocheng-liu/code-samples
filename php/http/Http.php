<?php

class Http {
    private $_ch = NULL;

    public function __construct () {
        $this->_ch = curl_init();
    }
    
    public function __destruct () {
        curl_close($this->_ch);
    }

    public function sendRequest($url) {
        curl_setopt($this->_ch, CURLOPT_URL, $url);
        curl_setopt($this->_ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($this->_ch, CURLOPT_HEADER, 0);
        //curl_setopt($ch, CURLOPT_POSTFIELDS, $postData );
        return curl_exec($this->_ch);
    }
   
    public function postRequest($url, array $postData) {
        curl_setopt($this->_ch, CURLOPT_URL, $url);
        curl_setopt($this->_ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($this->_ch, CURLOPT_HEADER, 0);
        curl_setopt($this->_ch, CURLOPT_POSTFIELDS, $postData );
        return curl_exec($this->_ch);
    }
    
    public function getRequestParam($key, $defaultValue, $cookie = false, $outTime = 864000) {
        if (isset($_GET[$key])) {
            setcookie($key, $_GET[$key], time() + $outTime);
            return  $_GET[$key] ;
        } elseif ($cookie && isset($_COOKIE[$key])) {
            return $_COOKIE[$key];
        } else {
            return $defaultValue;
        }
    }
}
