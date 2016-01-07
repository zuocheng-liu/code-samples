<?php
class UtilPage {
    protected $_pageSize = 0; // number of records in one page
    protected $_pageNo = 1;   // the number of the current page
    protected $_totalNum = 0; // total number of records
    protected $_totalPageNum = 0; // total number of pages
    protected $_pageListLength = 10;  //display max length of page list


    public function setPageListLength($pageListLength)
    {
        $this->_pageListLength = $pageListLength;
    }


    public function setTotalPageNum($totalPageNum)
    {
        $this->_totalPageNum = $totalPageNum;
    }

    public function getTotalPageNum()
    {
        $this->_totalPageNum = intval($this->_totalNum / $this->_pageSize);
        if (!($this->_totalNum % $this->_pageSize)) {
            $this->_totalPageNum = intval($this->_totalNum / $this->_pageSize) + 1;
        }
        return $this->_totalPageNum;
    }
    function __construct() {
        $this->_requestUrl = dirname('http://'.$_SERVER['SERVER_NAME'].':'.$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"]);
    }
    public function getTotalNum()
    {
        return $this->_totalNum;
    }
    protected $_requestUrl = null;

    public function setRequestUrl($requestUrl)
    {
        $this->_requestUrl = $requestUrl;
    }

    public function getRequestUrl()
    {
        return $this->_requestUrl;
    }
    public function setTotalNum($total) {
        $this->_totalNum = $total;
    }
    public function setPageSize($pageSize) {
        $this->_pageSize = $pageSize;
    }
    public function setPageNo($pageNum) {
        $this->_pageNo = $pageNum;
    }
    public function getOffset() {
        return $this->_pageSize * ($this->_pageNo - 1);
    }
    public function getLimit() {
        return $this->_pageSize;
    }
    public function getUrlByPage($pageNo,$keyName = 'pageNum') {
        if ($pageNo < 1) {
            $pageNo = 1;
        }
        if ($pageNo > $this->getTotalPageNum()) {
            $pageNo = $this->getTotalPageNum();
        }
        return $this->getRequestUrl() .'?'. http_build_query(array_merge($_GET,$_POST,array($keyName => $pageNo)));
    }
    public function getNextPageUrl() {
        return $this->getUrlByPage($this->getPageNo() + 1);
    }
    public function getLastPageUrl() {
        return $this->getUrlByPage($this->getTotalPageNum());
    }
    public function getFirstPageUrl() {
        return $this->getUrlByPage(1);
    }
    public function getPreviousPageUrl() {
        return $this->getUrlByPage($this->getPageNo() - 1);
    }
    public function getPageListLength()
    {
        return $this->_pageListLength;
    }
    public function getPageNo() {
        return intval($this->_pageNo);
    }
    public function getMaxDisplayPageNo()
    {
        if ($this->getTotalPageNum() <= $this->getPageListLength()) {
            return $this->getTotalPageNum();
        }
        if ($this->getPageNo() <= $this->getPageListLength()/2) {
            return $this->getPageListLength();
        }
        if ($this->getPageNo() + $this->getPageListLength()/2 >= $this->getTotalPageNum()) {
            return $this->getTotalPageNum();
        }
        return $this->getPageNo() + $this->getPageListLength()/2 - 1;
    }

    public function getMinDisplayPageNo()
    {
        if ($this->getTotalPageNum() <= $this->getPageListLength()) {
            return 1;
        }

        if ($this->getPageNo() <= $this->getPageListLength()/2) {
            return 1;
        }
        if ($this->getPageNo() + $this->getPageListLength()/2 > $this->getTotalPageNum()) {
            return $this->getTotalPageNum() - $this->getPageListLength();
        }
        return $this->getPageNo() - $this->getPageListLength()/2 + 1;
    }
}