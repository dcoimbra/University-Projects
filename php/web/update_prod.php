<html>
    <head>
        <link rel="stylesheet" type="text/css" href="styles.css">
    </head>
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

            #echo("<p>$sql_apagar_produto</p>");
            $db->query($sql_apagar_produto);
            echo("<p>Produto $ean removido</p>");
        }

        else {
            $design = $_REQUEST['design'];
            $categoria = $_REQUEST['categoria'];
            $forn_primario = $_REQUEST['forn_primario'];
            $forn_primario_nome = $_REQUEST['forn_primario_nome'];
            $forn_secundarios = $_REQUEST['forn_secundarios'];
            $forn_secundarios_nomes = $_REQUEST['forn_secundarios_nomes'];
            $data = $_REQUEST['data'];

            //verifica se fornecedor primario != fornecedores secundarios
            if (in_array($forn_primario, $forn_secundarios)) {

                exit("<p>$forn_primario ja e' fornecedor primario deste produto, pelo que nao pode ser secundario. 
                         Escolha outro fornecedor secundario.</p>");
            }

            //se o fornecedor for novo, adiciona-lo
            $fornecedores = $db->query("SELECT nif FROM fornecedor;");
            $fornecedores = $fornecedores->fetchAll(PDO::FETCH_COLUMN, 'nif');

            $fornecedores_candidatos = array_merge([$forn_primario], $forn_secundarios);
            $fornecedores_candidatos_nomes = array_merge([$forn_primario_nome], $forn_secundarios_nomes);

            $index = 0; //para encontrar o nome correspondente
            foreach ($fornecedores_candidatos as $candidato) {

                if (!in_array($candidato, $fornecedores) and $candidato != "") {

                    $sql_inserir_fornecedor = "INSERT INTO fornecedor(nif, nome) VALUES ('$candidato', '$fornecedores_candidatos_nomes[$index]')";

                    #echo("<p>$sql_inserir_fornecedor</p>");
                    $db->query($sql_inserir_fornecedor);
                }

                $index++;
            }

            //inserir produto
            $sql_inserir_produto = "INSERT INTO produto(ean, design, categoria, forn_primario, data)
                                    VALUES (
                                            '$ean',
                                            '$design',
                                            '$categoria',
                                            '$forn_primario',
                                            '$data'
                                            );";

            echo("<p>$sql_inserir_produto</p>");
            $db->query($sql_inserir_produto);

            //inserir relacao forn_secundario->produto
            foreach ($forn_secundarios as $f2) {

                if ($f2 != "") {
                    $sql_inserir_fornece_sec = "INSERT INTO fornece_sec(nif, ean) VALUES ('$f2', '$ean');";

                    #echo("<p>$sql_inserir_fornece_sec</p>");
                    $db->query($sql_inserir_fornece_sec);
                }
            }

            echo("<p>Produto $ean adicionado com designacao $design</p>");
        }

        $db->query("commit;");

        $db = null;
    }

    catch (PDOException $e)
    {
        $db->query("rollback;");

        $message = $e->getMessage();

        if (strpos($message, 'produto_ean_check') !== false) {

            echo ("<p>O EAN de um produto deve ter exatamente 13 algarismos.</p>");
        }

        else if (strpos($message, 'fornecedor_nif_check') !== false) {

            echo ("<p>O NIF de um produto deve ter exatamente 9 algarismos.</p>");
        }

        else {

            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
    }
    ?>
    <p><a href="index.html" class = "home">Menu inicial</a></p>

    </body>
</html>
