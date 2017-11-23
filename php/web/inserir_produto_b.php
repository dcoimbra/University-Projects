<html>
    <body>
    <h3>Inserir novo produto</h3>
    <form action="update_prod.php" method="post">
        <p>EAN: <input type="number" name="ean"/></p>
        <p>Designacao: <input type="text" name="design"/></p>

        <p>Categoria: <select name="categoria" Id="categoria">
            <option value=""></option>
            <?php
            try {
                $host = "db.ist.utl.pt";
                $user = "ist426008";
                $password = "northernlights";
                $dbname = $user;

                $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

                $categorias = $db->query("SELECT nome FROM categoria");
                foreach($categorias as $row) {
                    echo ("<option value='{$row['nome']}'>{$row['nome']}</option>");}
            }

            catch (PDOException $e)
            {
                echo("<p>ERROR: {$e->getMessage()}</p>");
            }
            ?>

            </select></p>

        <p>Fornecedor primario: <input type="text" name="forn_primario"/></p>
        <p>Data: <input type="date" name="data"/></p>
        <p><input type="hidden" name="tipo" value="inserir"/></p>
        <p><input type="submit" value="Submit"/></p>
    </form>
    </body>
</html>