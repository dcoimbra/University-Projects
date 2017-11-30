<html>
    <link rel="stylesheet" href="styles.css" type="text/css">
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

            echo("<table border=\"0\" cellspacing=\"5\">");

            echo("<tr>\n
                    <th>Operador</th>
                    <th>Instante</th>
                    <th>Unidades</th>
                  </tr>\n");

            foreach ($result as $row) {

                echo("<tr>");
                echo("<td>{$row['operador']}</td>");
                echo("<td>{$row['instante']}</td>");
                echo("<td>{$row['unidades']}</td>");
                echo("</tr>");
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
