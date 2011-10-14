<?php

function _aw_migrate_get_uid($user_name) {
  
  $uid = NULL;
     
  $query = db_select('users', 'u')
      ->fields('u', array('uid'))
      ->condition('name', $user_name)
      ->execute();

  foreach ($query as $user) {
      $uid = $user->uid;
  }    
  
  return $uid;

}




function _aw_migrate_clean_text($text, $oid = NULL) {   
    
  $ap = new aliasProcessor();
  $ap->oid = $oid;
  $text = preg_replace_callback(
    '|\#([a-zA-Z]+)([0-9]+)([a-zA-Z]{0,1})\#|', 
    array($ap, 'process'),
    $text
  );

    //   $text = str_replace(array('<BR>', '<br>', '</P>', '</p>', '</PRE>', '</pre>'), "\n", $text);
    //   $text = str_replace(array('<P>','<p>'), '', $text);
        
    // Allowing all HTML tags except html, body, div, span, fieldset, form, input, select, textarea, label, legend, font
    // pre is not allowed because of MSWord dumps
   
    $allowed_tags = array(
      'applet','object','iframe','h1','h2','h3','h4','h5','h6',
      'p','br','blockquote','a','abbr','acronym','address','big','cite','code',
      'del','dfn','em','img','ins','kbd','q','s','samp','small','strike',
      'strong','sub','sup','tt','var','b','u','i','center','dl','dt','dd','ol',
      'ul','li', 'table','caption','tbody','tfoot','thead','tr','th','td'
    );

    $text = filter_xss($text, $allowed_tags);
        
    $replace_map = array(
      '<strong><u>' => '<h3>',
      '</strong></u>' => '</h3>',
      '<u><strong>' => '<h3>',
      '</u></strong>' => '</h3>',
      '<i>' => '<em>',
      '</i>' => '</em>',
//      '<P>' => '',
//      '<p>' => '',
      '<STRONG>' => '<strong>',
      '</STRONG>' => '</strong>',
      '<b>' => '<strong>',
      '</b>' => '</strong>',
      '&nbsp;' => '',
      '<strong></strong>' => '',
      '<p></p>' => '',
      '<p><br />' => '<p>',
      '<br /><br />' => '<p />',
      'target="_blank" ' => '',
      '&ndash;' => '-',
      '&rsquo;' => "'",
      '&lsquo;' => "'",
      '&quot;' => '"',
      '&ldquo;' => '"',
      '&rdquo;' => '"',
      '&bdquo;' => '"',
      '&#8222;' => '"',
      '&#8220;' => '"',
      '&#8221;' => '"',
      '&acute;' => '´',
      '&Scaron;' => 'Š',
      '&scaron;' => 'Š',
      'ð' => 'ž',
      '¸' => 'ž',
      '&Otilde;' => 'Õ',
      '&otilde;' => 'õ',
      '&Auml;' => 'Ä',
      '&auml;' => 'ä',
      '&ouml;' => 'Ö',
      '&Ouml;' => 'Ö',
      '&uuml;' => 'ü',
      '&Uuml;' => 'Ü',
      '&#8722;' => '−',
      '&#8211;' => '–',
      '<A ' => '<a ',
      '</A>' => '</a>',
      '«' => '"',
      '»' => '"',
      
    );
  
    $text = str_replace(array_keys($replace_map), array_values($replace_map), $text);


    return $text;

}


class aliasProcessor {
  public $oid = NULL;
  public function process($matches) {
  
    switch ($matches[1]) {

      case 'file':
 
      $connection = Database::getConnection('default', 'aw');
      $query = $connection->query(
        'SELECT aw_f.file AS file  
        FROM {aliases} aw_a 
        LEFT OUTER JOIN {files} aw_f ON aw_a.target = aw_f.id 
        WHERE aw_a.source = :source AND aw_a.idx = :idx AND aw_a.type = :type',
        array(
          ':source' => $this->oid, 
          ':idx' => $matches[2],
          ':type' => 41
          )
        );

      foreach ($query as $result) {
        $filename = basename($result->file);
      }   
      
      
      $query = db_query(
        'SELECT fid  
        FROM {file_managed}  
        WHERE filename = :filename',
        array(
          ':filename' => $filename, 
          )
        );

      foreach ($query as $result) {
        $fid = $result->fid;
      }
      if ($fid) {
        $output = $filename; // @TODO figure out the proper link
      } else {
        $output = $matches[0];
      }
       
      break;
      
      case 'pict':
 
        $connection = Database::getConnection('default', 'aw');
        $query = $connection->query(
          'SELECT aw_i.file AS file  
          FROM {aliases} aw_a 
          LEFT OUTER JOIN {images} aw_i ON aw_a.target = aw_i.id 
          WHERE aw_a.source = :source AND aw_a.idx = :idx AND aw_a.type = :type',
          array(
            ':source' => $this->oid, 
            ':idx' => $matches[2],
            ':type' => 6
            )
          );

        foreach ($query as $result) {
          $filename = basename($result->file);
        }   

        $query = db_query(
          'SELECT fid  
          FROM {file_managed}  
          WHERE filename = :filename',
          array(
            ':filename' => $filename, 
            )
          );
 
        foreach ($query as $result) {
          $fid = $result->fid;
        }
        
        $image_info = array(
          'type' => 'media',
          'view_mode' => 'media_large',
          'fid' => $fid,
          'attributes' => array(
            'alt' => '',
            'title' => '',
            'class' => 'media-image',
            'typeof' => 'foaf:Image',
            'wysiwyg' => 1,
          ),
        );
        
        $output = $fid ? '[[' . drupal_json_encode($image_info) . ']]' : $matches[0];
         
        break;
 
        case 'link':

          $connection = Database::getConnection('default', 'aw');
          $query = $connection->query(
            'SELECT aw_a.target AS target, aw_e.url AS url, aw_e.name AS name  
            FROM {aliases} aw_a 
            LEFT OUTER JOIN {extlinks} aw_e ON aw_a.target = aw_e.id 
            WHERE aw_a.source = :source AND aw_a.idx = :idx AND aw_a.type = :type',
            array(
              ':source' => $this->oid, 
              ':idx' => $matches[2],
              ':type' => 21
              )
            );

          foreach ($query as $result) {
            $target = $result->target;
            $url = $result->url;
            $name = $result->name;
          }   

          $output = l($name ? $name : $url, $url); // @TODO if no name in extlinks table, try objects table
          break;      

  
      default:
        $output = $matches[0]; // Pass through
        break;
    }
    
    return $output;
  }
}
