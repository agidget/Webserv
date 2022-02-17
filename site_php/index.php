<!DOCTYPE html>
<html language="ru">
<head>
  <title>Cat</title>
  <link rel="icon" type="image/x-icon" href="../cat_face.png">
  <link rel="stylesheet" href="style.css">
</head>
<body>
  <main>
    <section class="cats">

      <div class="wrapper">
        <h2 class="cats-h2">Which cat are you today?</h2>

        <div class="cats-container">
          <a href="meow.html" target="_blank" style="text-decoration:none" class="cat-img">
            <img src="images/100.jpg" alt="cat image">
            <h3>CAT 100</h3>
            <p>Continue</p>
          </a>
          <a href="meow.html" target="_blank" style="text-decoration:none" class="cat-img">
            <img src="images/101.jpg" alt="cat image">
            <h3>CAT 101</h3>
            <p>Switching Protocols</p>
          </a>
          <a href="meow.html" target="_blank" style="text-decoration:none" class="cat-img">
            <img src="images/102.jpg" alt="cat image">
            <h3>CAT 102</h3>
            <p>Processing</p>
          </a>
          <a href="meow.html" target="_blank" style="text-decoration:none" class="cat-img">
            <img src="images/200.jpg" alt="cat image">
            <h3>CAT 200</h3>
            <p>OK</p>
          </a>
          <a href="meow.html" target="_blank" style="text-decoration:none" class="cat-img">
            <img src="images/201.jpg" alt="cat image">
            <h3>CAT 201</h3>
            <p>Created</p>
          </a>
          <a href="meow.html" target="_blank" style="text-decoration:none" class="cat-img">
            <img src="images/202.jpg" alt="cat image">
            <h3>CAT 202</h3>
            <p>Accepted</p>
          </a>
          <a href="meow.html" target="_blank" style="text-decoration:none" class="cat-img">
            <img src="images/203.jpg" alt="cat image">
            <h3>CAT 203</h3>
            <p>Non-Authoritative Information</p>
          </a>
        </div>

        <div class="cats-upload">
          <form action="upload.php" method="POST" enctype="multipart/form-data">
            <input type="text" name="filename" placeholder="File name"><br>
            <input type="text" name="filetitle" placeholder="Image title"><br>
            <input type="text" name="filedesc" placeholder="Image description"><br><br>
            <input type="file" name="file"><br><br>
            <button type="submit" name="submit">UPLOAD</button>
          </form>
        </div>

        </div>
    </section>
  </main>
</body>

</html>