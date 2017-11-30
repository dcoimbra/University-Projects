<html>
    <body>
    <h3>Inserir nova super categoria </h3>
    <form name="insert" action="update_cat.php" method="post">
        <p>Nome: <input type="text" name="nome"/></p>

        <?php
        try {
            $host = "db.ist.utl.pt";
            $user = "ist157842";
            $password = "kikoki";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        
            $categorias = $db->query("SELECT nome FROM categoria;");

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
    </form>
    </body>
</html>