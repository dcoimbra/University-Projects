<html>
    <head>
        <link rel="stylesheet" type="text/css" href="styles.css">
    </head>
    <body>
    <?php
    $tipo = $_REQUEST['tipo'];
    $nome = $_REQUEST['nome'];

    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist426008";
        $password = "northernlights";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $db->query("start transaction;");

        
        $sql = "SELECT ean FROM produto WHERE categoria = ('$nome');";
        $categ = $db->query($sql);
        
        foreach ($categ as $ean) {
           $sql_p = "UPDATE produto SET categoria = null WHERE ean = '{$ean['ean']}';";
           $db->query($sql_p);
           echo("<p>Producto com ean {$ean['ean']} referencia a categoria a apagar.<br> A categoria deste produto foi removida e encontra-se vazia.</p>");
        }

        $sql1 = "SELECT super_categoria FROM constituida WHERE categoria = ('$nome');"; //a super-categoria associada a' categoria, se existir

        $super = $db->query($sql1);
        $contS = $super->rowCount();

        if($contS) {  //se existir uma relacao entre a categoria e uma super categoria:

            $row = $super->fetch();
            
            $sql2 = "SELECT * FROM constituida WHERE super_categoria = '{$row['super_categoria']}' AND categoria != '$nome';";

            $subcats_from_super = $db->query($sql2);
            $cont = $subcats_from_super->rowCount();

            if ($cont == 0){ //se essa super categoria so' estiver ligada 'a categoria a apagar:
                echo("<p><p>ERRO: Super categoria {$row['super_categoria']} associada apenas a $nome. Remova {$row['super_categoria']} primeiro.</p></p>");
                $db->query("rollback;");
                return;
            }

            $sql = "DELETE FROM constituida WHERE categoria = '$nome';";
            $db->query($sql);
        }

        if($tipo == "simples") {
            $sql = "DELETE FROM categoria_simples WHERE nome = '$nome';";
            $db->query($sql);
        }
        elseif($tipo == "super") {
            $sql = "DELETE FROM constituida WHERE super_categoria = '$nome';";
            $db->query($sql);
            $sql = "DELETE FROM super_categoria WHERE nome = '$nome';";
            $db->query($sql);
        }

        #echo("<p>$sql</p>");
        $sql = "DELETE FROM categoria WHERE nome = '$nome';";
        $db->query($sql);


        echo("<p>Categoria $nome removida");

        $db->query("commit;");

        $db = null;
    }

    catch (PDOException $e)
    {
        $db->query("rollback;");
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }
    ?>

    <p><a href="index.html" class = "home">Menu inicial</a></p>

    </body>
</html>
