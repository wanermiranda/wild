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
    if ($descriptor == "bic" || $descriptor == "sasi" || $descriptor == "eoac"){
        $filename = $_FILES["Imagem"]["name"]; 
        if($_FILES["Imagem"]["type"] !== "image/jpeg" &&
           $_FILES["Imagem"]["type"] !== "image/jpg" &&
           $_FILES["Imagem"]["type"] !== "image/png"){
            echo "Formato da Imagem invalido. Utilizar jpg, ppm ou png \n";
        }
        
        $uploaddir = 'images/';
        $uploadimage = $uploaddir . $filename; 
        move_uploaded_file($_FILES['Imagem']['tmp_name'], $uploadimage);
        
        //$converted_image = str_replace("png", "ppm",str_replace("jpg", "ppm",$uploadimage));
        //$convert = "/usr/bin/convert ".$uploadimage. " " . $converted_image;
        //echo $convert . "<br>";
        //exec($convert, $out1);
    
        //$extract_cmd = "extractors/".$descriptor."_extraction ".$converted_image. " hist/bic.bin";
        //echo $extract_cmd . "<br>";
        //exec($extract_cmd, $out1);

        $predict_cmd = "predict/predict ".$descriptor. " svm_states/train1.".$descriptor.".xml" . " images/". $filename;
        echo $predict_cmd . "<br>";
        exec($predict_cmd, $out2);
        echo $out2[0];
        
    //echo $out2[0] . "<br>";
    }
?>
<LABEL>Imagem:</LABEL>
<img src="http://tp3icv.atspace.cc/images/<?php echo $filename?>" alt="Imagem" >
</DIV>
</BODY>