<html>
    <body>
    <?php
    $tipo = $_REQUEST['tipo'];
    $nome = $_REQUEST['nome'];

    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist157842";
        $password = "kikoki";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $db->query("start transaction;");

        if ($tipo == "simples") {

            $sql_inserir_cat1 = "INSERT INTO categoria(nome) VALUES('$nome');";
            $sql_inserir_cat2 = "INSERT INTO categoria_simples(nome) VALUES('$nome');";

            echo("$sql_inserir_cat1\n");
            $db->query($sql_inserir_cat1);
            echo("<p>$sql_inserir_cat2\n");
            $db->query($sql_inserir_cat2);
            echo("<p>Categoria $nome inserida\n</p>");
        }

        elseif ($tipo == "super") {

            $cat_s = $_REQUEST['cat_s'];
            
            if(empty($cat_s)){
                $db->query("rollback;");
                echo("<p>ERRO: Tem de seleccionar pelo menos uma categoria simples.</p>");
                return;
            }

            $sql_inserir_cat1 = "INSERT INTO categoria(nome) VALUES('$nome');";
            $sql_inserir_cat2 = "INSERT INTO super_categoria(nome) VALUES('$nome');";
            echo("$sql_inserir_cat1\n");
            $db->query($sql_inserir_cat1);
            echo("<p>$sql_inserir_cat2\n");
            $db->query($sql_inserir_cat2);


            foreach($cat_s as $cat) {
                $sql = "INSERT INTO constituida(super_categoria, categoria) values('$nome', '$cat');";
                $db->query($sql);
            }

            echo("<p>Categoria $nome inserida\n</p>");

  
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
