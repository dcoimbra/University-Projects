<html>
    <body>
        <?php
        try
        {
            $host = "db.ist.utl.pt";
            $user ="ist426008";
            $password = "northernlights";
            $dbname = $user;
            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $ean = $_REQUEST['ean'];

            echo("<h3>Listar eventos de reposicao do produto '$ean'</h3>");

            $sql = "SELECT operador, instante, unidades FROM reposicao WHERE ean = '$ean';";

            $result = $db->query($sql);

            echo("<table border=\"0\" cellspacing=\"5\">\n");

            echo("<tr>\n
                    <th>Operador</th>\n
                    <th>Instante</th>\n
                    <th>Unidades</th>\n
                  </tr>\n");

            foreach ($result as $row) {

                echo("<tr>\n");
                echo("<td>{$row['operador']}</td>\n");
                echo("<td>{$row['instante']}</td>\n");
                echo("<td>{$row['unidades']}</td>\n");
                echo("</tr>\n");
            }
            echo("</table>\n");

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
