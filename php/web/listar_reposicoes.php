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

            //Recebe ean do produto cujos eventos de reposicao vao ser listados
            $ean = $_REQUEST['ean'];

            echo("<h3>Lista dos eventos de reposicao do produto '$ean'</h3>");

            //Tabela dos eventos de reposicao do produto
            $sql = "SELECT operador, instante, unidades FROM reposicao WHERE ean = '$ean';";
            $result = $db->query($sql);

            echo("<table border=\"0\" cellspacing=\"5\">");

                //Table headers
            echo("<tr>
                    <th>Operador</th>
                    <th>Instante</th>
                    <th>Unidades</th>
                  </tr>");

                //Table content
            foreach ($result as $row) {

                echo("<tr>
                        <td>{$row['operador']}</td>
                        <td>{$row['instante']}</td>
                        <td>{$row['unidades']}</td>
                    </tr>");
            }
            echo("</table>");

            $db = null;

            $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
            echo "<p><a href='$url' class='Back'>Voltar</a></p>";

            echo("<p><a href=\"index.html\" class = 'home'>Menu inicial</a></p>");
        }
        catch (PDOException $e)
        {
            $db->query("rollback;");
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
        ?>

    </body>
</html>
