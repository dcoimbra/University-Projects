<html>
    <head>
        <link rel="stylesheet" href="styles.css" type="text/css">
    </head>
    <body>
    <h3>Inserir nova super categoria </h3>
    <form name="insert" action="update_cat.php" method="post">
        <p>Nome: <input type="text" name="nome" required/></p>

        <?php
        try {
            $host = "db.ist.utl.pt";
            $user ="ist426008";
            $password = "northernlights";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        
            $categorias = $db->query("SELECT * FROM categoria WHERE nome NOT IN(SELECT categoria FROM constituida);");
            
            if($categorias->rowCount() == 0) {
                echo("ERRO: Nao existem sub-categorias disponiveis, por favor crie uma categoria simples primeiro.<p>");
                
                $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
                echo "<a href='$url'>Voltar</a>";
                return;
            }
           
            echo("Seleccione as categorias que constituem esta super categoria\n<p>");

            foreach($categorias as $row) {

                echo ("<input type=\"checkbox\" name=\"cat_s[]\" value='{$row['nome']}'/>{$row['nome']}<br>");
            }
        }

        catch (PDOException $e)
        {
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
        ?>
        <p><input type="hidden" name="tipo" value="super"/></p>
        <p><input type="submit" value="Submit"/></p>
        <?php
            $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
            echo "<a href='$url' class = Back>Voltar</a>";
        ?>
    </form>
    </body>
</html>