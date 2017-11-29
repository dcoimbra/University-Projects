<html>
    <body>
    <?php
        $ean = $_REQUEST['ean'];
        $design = $_REQUEST['design'];
        try
        {
            $host = "db.ist.utl.pt";
            $user ="ist426008";
            $password = "northernlights";
            $dbname = $user;
            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $db->query("start transaction;");

            $sql = "UPDATE produto SET design = '$design' WHERE ean = '$ean';";

            echo("<p>$sql</p>");

            $db->query($sql);

            $db->query("commit;");
            echo("<p>Descricao do produto $ean alterado para $design</p>");

            $db = null;
        }
        catch (PDOException $e)
        {
            $db->query("rollback;");
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
    ?>
    </body>
</html>
