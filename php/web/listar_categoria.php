<html>
    <body>
    <?php

    $nome = $_REQUEST['nome'];
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

        $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
        echo "<a href='$url' class='Back'>Voltar</a>";

        $db = null;

    }

    catch (PDOException $e)
    {
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }

    function list_aux($cat, $db) {
        global $aux;

        $sql = "SELECT categoria FROM constituida WHERE super_categoria = '$cat';";
//       echo("<p>$sql\n</p>");
        $subcats = $db->query($sql);
        $cont = $subcats->rowCount();
        
        if($cont == 0)
                return;
        
        //Listar as subcategorias
        echo("<ul>");
        foreach ($subcats as $row) {

            echo("<li>{$row['categoria']}</li>");

            list_aux($row['categoria'], $db);
        }
        echo("</ul>");
    }
    
    ?>
    </body>
</html>
