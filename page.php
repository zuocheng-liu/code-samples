<div class="pagination pagination-centered">
    <?php
    if (1 < $pageObj->getPageNo()) {
        echo " <li><a class=\"prev\" href=". $pageObj->getPreviousPageUrl() . ">上一页</a></li>\n";
    }

    for($i=$pageObj->getMinDisplayPageNo();$i<=$pageObj->getMaxDisplayPageNo();$i++) {
        if ($i == $pageObj->getPageNo()) {
            echo "<li><a class=\"current\" href=". $pageObj->getUrlByPage($i) .">$i</a></li>\n";
        } else {
            echo "<li><a href=". $pageObj->getUrlByPage($i) .">$i</a></li>\n";
        }
    }

    if ($pageObj->getTotalPageNum() > $pageObj->getPageNo()) {
        echo "<li><a class=\"next\" href=". $pageObj->getNextPageUrl().">下一页</a></li>\n";
    }
    ?>
</div>