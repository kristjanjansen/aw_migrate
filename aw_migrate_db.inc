<?php

class AWUserMigration extends Migration {
  public function __construct() {

    parent::__construct();

    $query = db_select('groups', 'a_g')
             ->fields('a_g', array('gid', 'name'))
             ->condition('type', 1)
             ->distinct();
             
             
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    
    $this->map = new MigrateSQLMap($this->machineName,
        array('gid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );

 //   $this->addFieldMapping('uid', 'gid');
    $this->addFieldMapping('name', 'name');
}

public function prepareRow($current_row) {
}

}

class AWDocMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
        
    $query = db_select('objects', 'a_o')
             ->fields('a_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status', 'metadata'))
             ->condition('class_id', 7)
             ->orderBy('modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query);           
    $this->destination = new MigrateDestinationNode('article');
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array('type' => 'int',
                        'unsigned' => TRUE,
                        'not null' => TRUE,
                        )
        ),
        MigrateDestinationNode::getKeySchema()
      );
  
    
    $this->addFieldMapping('nid', 'oid');
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('body', 'body')->arguments(array('format' => 'full_html'));

  }

  public function prepareRow($current_row) {
           
     $query = db_select('users', 'u')
        ->fields('u', array('uid'))
        ->condition('name', $current_row->createdby)
        ->execute();
      
     foreach ($query as $user) {
       $current_row->uid = $user->uid;
     }
    
     $query = db_select('documents', 'a_d')
         ->fields('a_d', array('content'))
         ->condition('docid', $current_row->oid)
         ->execute();

      foreach ($query as $doc) {
        $current_row->body = $doc->content;
      }
  
   
 /*
    $file_index = NULL;
    $image_index = NULL;
    $gallery_index = NULL;
    
    $found = preg_match_all('#\#file([0-9]+)\##e',$current_row->content_filtered, $matches);
    if ($found) {
      foreach ($matches[1] as $match) {
        $file_index[] = $match;
      }
    }

    $found = preg_match_all('#\#pict([0-9]+)\##e',$current_row->content_filtered, $matches);
    if ($found) {
      foreach ($matches[1] as $match) {
        $image_index[] = $match;
      }
    }

    $found = preg_match_all('#\#minigalerii([0-9]+)\##e',$current_row->content_filtered, $matches);
    if ($found) {
      foreach ($matches[1] as $match) {
        $gallery_index[] = $match;
      }
    }
    
    // References "('[file $1]')" 
    
    $preg_replace_map = array(
  //    '#\#file([0-9]+)\##e' => '',
  //    '#\#pict([0-9]+)\##e' => '',
  //    '#\#minigalerii([0-9]+)\##e' => '',
      '#\s\s+#' => ' ',
      '#<span style=\"font-weight: bold;\">(.*)</span>#e' => "('<strong>$1</strong>')",
      '#<!--(.*)-->#e' => '',
      '#<strong>([\s]+)</strong>#e' => '',
      '#<strong>GALLERY</strong>#e' => '',
    );
    
    $current_row->content_filtered = preg_replace(array_keys($preg_replace_map), array_values($preg_replace_map), $current_row->content_filtered);


    // Omitting html, body, div, span, fieldset, form, input, select, textarea, label, legend, font

      $allowed_tags = array(
      'applet','object','iframe','h1','h2','h3','h4','h5','h6',
      'p','blockquote','pre','a','abbr','acronym','address','big','cite','code',
      'del','dfn','em','img','ins','kbd','q','s','samp','small','strike',
      'strong','sub','sup','tt','var','b','u','i','center','dl','dt','dd','ol',
      'ul','li', 'table','caption','tbody','tfoot','thead','tr','th','td'
      );

      $current_row->content_filtered = filter_xss($current_row->content_filtered, $allowed_tags);



    $replace_map = array(
      '<b>' => '<strong>',
      '</b>' => '</strong>',
      '&nbsp;' => '',
      '<strong></strong>' => '',
      '<p></p>' => '',
      '<p><br />' => '<p>',
      'target="_blank" ' => '',
      '&ndash;' => '-',
      '&rsquo;' => "'",
      '&lsquo;' => "'",
      '&ldquo;' => '"',
      '&rdquo;' => '"',
      '&bdquo;' => '"',
      '&acute;' => '´',
      '&Scaron;' => 'Š',
      '&scaron;' => 'Š',
      '&Otilde;' => 'Õ',
      '&otilde;' => 'õ',
      '&Auml;' => 'Ä',
      '&auml;' => 'ä',
      '&Ouml;' => 'Ö',
      '&ouml;' => 'Ö',
      '&Uuml;' => 'Ü',
      '&uuml;' => 'ü',
    );
  
    $current_row->content_filtered = str_replace(array_keys($replace_map), array_values($replace_map), $current_row->content_filtered);

 */


  }

}

function _aw_migrate_convert_file_path($path, $id = NULL, $type, $domain) {  
  $new_path = NULL;
  $path_elements = explode('/', $path);
  $file = str_replace(' ', '+', $path_elements[count($path_elements)-1]);
  switch ($type) {
    case 'file':
      $new_path = $domain . '/orb.aw/class=file/action=preview/id=' . $id . '/' . $file;
      break;
    case 'image':
      $new_path = $domain . '/orb.aw/class=image/action=show/fastcall=1/file=' . $file;
      break;
  }
  return $new_path;
}
