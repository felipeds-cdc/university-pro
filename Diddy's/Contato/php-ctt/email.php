<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Contato</title>
    <script src="https://cdn.jsdelivr.net/npm/sweetalert2@11"></script>
</head>
<body>


<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verifique se os campos estão preenchidos
    
   
if (!empty($_POST["name"]) && !empty($_POST["email"]) && !empty($_POST["mensagem"])) {
        $nome = htmlspecialchars($_POST["name"]);
        
       
$email = htmlspecialchars($_POST["email"]);
        $mensagem = htmlspecialchars($_POST["mensagem"]);
        
        echo "<script>
                Swal.fire('Mensagem Enviada!', 'Sua mensagem foi enviada com sucesso.', 'success');
              </script>";
    } 
    } 


 
?>

</body>
</html>