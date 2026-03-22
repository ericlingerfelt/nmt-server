<?php
  echo "<html><head></head><body>";
  echo '<div style="text_align:center;margin: 50px 100px;">';
  echo '<form action="mass_main.php" method="POST">';
  echo '<input type="text" name="ACTION" value="' . $_POST['ACTION'] . '">';
  echo '<br><input type="text" name="PASSWORD" value="';
  echo sha1($_POST['PASSWORD']) . '">';
  echo '<br><input type="text" name="USER" value="' . $_POST['USER'] . '">';
  echo '<br><input type="submit" value="Submit">';
  echo "</form></div></body></html>";
?>
