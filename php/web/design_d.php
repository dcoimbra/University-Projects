<html>
    <head>
        <link rel="stylesheet" type="text/css" href="styles.css">
    </head>
    <body>
    <h3>Mudar designacao do produto <?=$_REQUEST['ean']?></h3>
    <form action="edit_design_d.php" method="post">
        <p><input type="hidden" name="ean" value="<?=$_REQUEST['ean']?>"/></p>
        <p>Nova designacao: <input type="text" name="design" autocomplete="off"/></p>
        <p><input type="submit" value="Submit"/></p>
    </form>
    <?php
        $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
        echo "<a href='$url' class ='Back'>Voltar</a>";
    ?>
    </body>
</html>
