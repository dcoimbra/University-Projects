<html>
    <body>
    <?php

    $nome = $_REQUEST['nome'];
    $aux = [];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist426008";
        $password = "northernlights";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


        echo("<b>Sub-categorias de $nome:\n</b>");
        
        list_aux($nome, $db);
      
        $db = null;

    }

    catch (PDOException $e)
    {
//        $db->query("rollback;");
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }

    function list_aux($cat, $db) {
        global $aux;

        $sql = "SELECT categoria FROM constituida WHERE super_categoria = '$cat';";
        #echo("<p>$sql\n</p>");
        $subcats = $db->query($sql);
        $cont = $subcats->rowCount();
        
        if($cont == 0)
                return;
        
        //Listar as subcategorias
        foreach ($subcats as $row) {

            if (in_array($row['categoria'], $aux)){
                continue;
            }

            echo("<ul>
                    <li>{$row['categoria']}</li>
                  </ul>");

            array_push($aux, $row['categoria']);
            list_aux($row['categoria'], $db);
        }

        $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
        echo "<a href='$url'>Voltar</a>";
    }
    
    ?>
    </body>
</html>
