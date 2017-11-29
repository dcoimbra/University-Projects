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

            $sql_apagar_produto = "DELETE FROM produto WHERE ean = '$ean';";

            echo("$sql_apagar_produto\n");
            $db->query($sql_apagar_produto);
            echo("Produto $ean removido\n");
        }

        else {
            $design = $_REQUEST['design'];
            $categoria = $_REQUEST['categoria'];
            $forn_primario = $_REQUEST['forn_primario'];
            $nome_forn_primario = $_REQUEST['nome_forn_primario'];
            $forn_secundario = $_REQUEST['forn_secundario'];
            $nome_forn_sec = $_REQUEST['nome_forn_sec'];
            $data = $_REQUEST['data'];

            if ($forn_primario == $forn_secundario) {

                exit("<p>Fornecedor primario nao pode ser igual ao secundario</p>");
            }

            $sql_inserir_produto = "INSERT INTO produto(ean, design, categoria, forn_primario, data)
                                    VALUES (
                                            '$ean',
                                            '$design',
                                            '$categoria',
                                            '$forn_primario',
                                            '$data'
                                            );";

            $sql_inserir_forn_sec = "INSERT INTO fornece_sec(nif, ean)
                                     VALUES (
                                              '$forn_secundario', 
                                              '$ean'
                                            );";

            echo("<p>$sql_inserir_produto</p>");
            echo("<p>$sql_inserir_forn_sec</p>");

            $db->query($sql_inserir_produto);
            $db->query($sql_inserir_forn_sec);

            echo("<p>Produto $ean adicionado com designacao $design</p>");
        }

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
