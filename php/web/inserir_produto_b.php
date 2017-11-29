<html>
    <body>
    <h3>Inserir novo produto</h3>
    <form name="insert" action="update_prod.php" method="post">
        <p>EAN: <input type="number" name="ean"/></p>
        <p>Designacao: <input type="text" name="design"/></p>

        <?php

        try {
            $host = "db.ist.utl.pt";
            $user = "ist426008";
            $password = "northernlights";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            echo("<p>Categoria: <select name=\"categoria\" Id=\"categoria\">");

            //Creation of a dropdown list
            $categorias = $db->query("SELECT nome FROM categoria;");

            foreach($categorias as $row) {
                echo ("<option value='{$row['nome']}'>{$row['nome']}</option>");
            }

            echo(" </select></p>");

            echo("<p>Fornecedor primario: <select name=\"forn_primario\" Id=\"categoria\">");

            //Creation of a dropdown list
            $fornecedores = $db->query("SELECT nif FROM fornecedor;");

            foreach($fornecedores as $row) {
                echo ("<option value='{$row['nif']}'>{$row['nif']}</option>");
            }

            echo(" </select></p>");


            echo("<p>Fornecedor secundario: <select name=\"forn_secundario\" Id=\"categoria\">");

            $fornecedores = $db->query("SELECT nif FROM fornecedor;");

            foreach ($fornecedores as $row) {

                echo("<option value='{$row['nif']}'>{$row['nif']}</option>");
            }

            echo(" </select></p>");
        }

        catch (PDOException $e)
        {
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
        ?>
        <p>Data: <input type="date" name="data"/></p>
        <p><input type="hidden" name="tipo" value="inserir"/></p>
        <p><input type="submit" value="Submit"/></p>
    </form>
    </body>
</html>