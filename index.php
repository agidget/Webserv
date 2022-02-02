<!-- <!DOCTYPE html>
<html>
<body style="background-color:aquamarine;">

<h1 style="color:indigo;"><center>This is some dummy index page!!!!</center></h1>
<p><center><b>Wow that's possible!!!!</b></center></p>
<?php
    echo 'Hello, world!<br/>';
    echo 'lol<br/>';
    echo date('d.m.Y');
?>

</body>
</html> -->

<?php

$myVar = 100;
$b = 14.12;
$c = "lol";
$e = array(
    "1,2,3",
    "John",
    "Meow"
);
$f = array(
    1 => "wow",
    2 => 20
);

echo $myVar;
echo '<br>';
echo $b;
echo '<br>';
echo $c;
echo '<hr>';
echo $e[0];
echo '<br>';
echo $f[1];
echo '<br>';
echo $c . ' ' . $f[1] . '<br>';

if ($myVar == 100)
{
    echo 'kek<br>';
}

foreach($e as $val)
{
    echo $val . '<br>';
}

function myfunction()
{
    echo 'lol<br>';
}
myfunction();

?>