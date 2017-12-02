<html>
    <script type="text/javascript" src="forn_sec_input.js"></script>
    <body>
    <h3>Inserir novo produto</h3>
    <form name="insert" action="update_prod.php" method="post">
        <p>EAN: <input type="number" name="ean" autocomplete="off" required/></p>
        <p>Designacao: <input type="text" name="design" autocomplete="off" required/></p>

        <?php

        try {
            $host = "db.ist.utl.pt";
            $user = "ist426008";
            $password = "northernlights";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            echo("<p>Categoria: <select name=\"categoria\" id=\"categoria\">");

            //Creation of a dropdown list
            $categorias = $db->query("SELECT nome FROM categoria;");

            foreach($categorias as $row) {
                echo ("<option value='{$row['nome']}'>{$row['nome']}</option>");
            }

            echo("</select></p>");

            echo("<datalist id=\"forn_list\">");

            //Creation of a dropdown list
            $fornecedores = $db->query("SELECT nif FROM fornecedor;");

            foreach($fornecedores as $row) {
                echo ("<option value='{$row['nif']}'>{$row['nif']}</option>");
            }

            echo("</datalist>");

            echo("<p>Fornecedor primario: NIF: <input list=\"forn_list\" name=\"forn_primario\" id=\"forn_primario\" autocomplete=\"off\" required>");
            echo(" Nome: <input name=\"forn_primario_nome\" id=\"forn_primario_nome\" autocomplete=\"off\"></p>");

            echo("<p>Fornecedor secundario: NIF: <input list=\"forn_list\" name=\"forn_secundarios[]\" id=\"forn_secundario\" autocomplete=\"off\" required>");
            echo(" Nome: <input name=\"forn_secundarios_nomes[]\" id=\"forn_secundarios_nomes\" autocomplete=\"off\"></p>");
            echo("<div id=\"input_container\"></div>");
            echo("<button type=\"button\" onclick=\"adicionarInput();\">Adicionar fornecedor secundario</button>");

        }
        
        catch (PDOException $e)
        {
            echo(" <p>ERROR: {$e->getMessage()}</p>");
        }
        ?>
        <p>Data: <input type="date" name="data" autocomplete="off"/></p>
        <p><input type="hidden" name="tipo" value="inserir" autocomplete="off"/></p>
        <p><input type="submit" value="Submit"/></p>

        <?php
            $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
            echo "<a href='$url'>Voltar</a>";
        ?>

    </form>
    </body>
</html>