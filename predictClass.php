<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<!doctype html>
<html>
<head>

<title>Processamento</title>

<STYLE>

body { COLOR:#FFF;FONT-FAMILY:arial;BACKGROUND:#7092be; }

label { MARGIN:15px 0 5px 0;DISPLAY:block; }

                        
</STYLE>

<link rel="shortcut icon" type="image/png" href="http://tp3icv.atspace.cc/favicon.png">
<link rel="icon" type="image/png" href="http://tp3icv.atspace.cc/favicon.png">

</head>
<BODY>
<DIV>   
<?php
    $descriptor = $_POST["Descritor"];
    if ($descriptor == "BIC" || $descriptor == "LCH"){
        $filename = 'imagem.jpg'; 
        if($_FILES["Imagem"]["type"] !== "image/jpeg" &&
           $_FILES["Imagem"]["type"] !== "image/jpg") {
            echo "Formato da Imagem inválido. Utilizar jpg";
        }
    }else{
        $filename = 'imagem.pgm';
        if($_FILES["Imagem"]["type"] !== "image/pgm") {
            echo "Formato da Imagem invalido. Utilizar pgm";
        }
    }
    if($_FILES["Mascara"]["type"] !== "image/pgm" &&
       $_FILES["Mascara"]["type"] !== "application/octet-stream") {
        echo "Formato da Mascara invalido. Utilizar pgm";
    }
    
    $uploaddir = 'images/';
    $uploadimage = $uploaddir . $filename; 
    move_uploaded_file($_FILES['Imagem']['tmp_name'], $uploadimage);

    $uploadmask = $uploaddir . 'mask.pgm'; 
    move_uploaded_file($_FILES['Mascara']['tmp_name'], $uploadmask);
    
    if ($descriptor == "BIC"){
        
        $run = "./extrai_bic ".$uploadimage. " " . $uploadmask . " hist/bic.bin";
        echo $run . "<br>";
        exec("./extrai_bic ".$uploadimage. " " . $uploadmask . " hist/bic.bin", $out1);
        //print_r($out1);
        $run = "./predict ".$descriptor. " svm/bic.xml" . " hist/bic.bin";
        echo $run . "<br>";
        exec("./predict ".$descriptor. " svm/bic.xml" . " hist/bic.bin", $out2);
        //print_r($out2);
        
    } elseif ($descriptor == "LCH"){
        exec("./extrai_lch ".$uploadimage. " " . $uploadmask . " hist/lch.bin", $out1);
        exec("./predict ".$descriptor. " svm/lch.xml" . " hist/lch.bin", $out2);
        
    } elseif ($descriptor == "UNSER"){
        exec("./extrai_unser ".$uploadimage. " " . $uploadmask . " hist/unser.bin", $out1);
        exec("./predict ".$descriptor. " svm/unser.xml" . " hist/unser.bin", $out2);
    }
    //echo $out2[0] . "<br>";
?>
<LABEL>Imagem:</LABEL>
<img src="http://tp3icv.atspace.cc/images/<?php echo $filename?>" alt="Imagem" >
</DIV>
</BODY>