<html>
    <body>
    <?php
    $tipo = $_REQUEST['tipo'];
    $ean = $_REQUEST['ean'];

    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist426008";
        $password = "northernlights";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $db->query("start transaction;");

        if ($tipo == "remover") {

            $sql  = "DELETE FROM produto WHERE ean = '$ean';";
        }

        else {
            $design = $_REQUEST['design'];
            $categoria = $_REQUEST['categoria'];
            $forn_primario = $_REQUEST['forn_primario'];
            $data = $_REQUEST['data'];

            $sql = "INSERT INTO produto(ean, design, categoria, forn_primario, data)
                    VALUES (
                            '$ean',
                             '$design',
                             '$categoria',
                             '$forn_primario',
                             '$data'
                    );";
        }
        echo("<p>$sql</p>");

        $db->query($sql);

        $db->query("commit;");

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
