<?php


class AWUserMigration extends Migration {
  public function __construct() {

    parent::__construct();

    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('groups', 'aw_g');
    
    $query->fields('aw_g', array('gid', 'name'))
          ->condition('type', 1)
          ->distinct();
        
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           

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

    $this->addFieldMapping('name', 'name');
    }

    public function prepareRow($current_row) {
    }

}



class AWImageMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $dir = 'public://aw_images';
    
    if (!file_prepare_directory($dir)) {
      drupal_mkdir($dir);
    }
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');

    $query->leftJoin('images', 'aw_i', 'aw_o.oid = aw_i.id');

    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status'))
      ->fields('aw_i', array('file'))
      ->condition('aw_o.class_id', 6)
      ->orderBy('aw_o.modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           
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

    $associated_file_arguments = MigrateFileFieldHandler::arguments(variable_get('file_public_path', conf_path() . '/files') . '/aw_images', 'file_copy', FILE_EXISTS_REPLACE, NULL, 2, 1, NULL, NULL, ';');
    
     $this->addFieldMapping('field_image', 'images')
       ->separator('|')
       ->arguments($associated_file_arguments);

    }

  public function prepareRow($current_row) {

    $current_row->uid = _aw_get_uid($current_row->createdby);
 
    $current_row->images = basename($current_row->file);
 
    if (variable_get('aw_file_copy', FALSE)) {
      _aw_copy_file($current_row->file, 'public://aw_images');
    } 
  
  }

}


class AWFileMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $dir = 'public://aw_files';
    
    if (!file_prepare_directory($dir)) {
      drupal_mkdir($dir);
    }
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');

    $query->leftJoin('files', 'aw_f', 'aw_o.oid = aw_f.id');

    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status'))
      ->fields('aw_f', array('file'))
      ->condition('aw_o.class_id', 41)
      ->orderBy('aw_o.modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           
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

    $associated_file_arguments = MigrateFileFieldHandler::arguments(variable_get('file_public_path', conf_path() . '/files') . '/aw_files', 'file_copy', FILE_EXISTS_REPLACE, NULL, 2, 1, NULL, NULL, ';');
    
     $this->addFieldMapping('field_file', 'files')
       ->separator('|')
       ->arguments($associated_file_arguments);

    }

  public function prepareRow($current_row) {
 
    $current_row->uid = _aw_get_uid($current_row->createdby);
  
    $current_row->files = basename($current_row->file);
    
    if (variable_get('aw_file_copy', FALSE)) {
      _aw_copy_file($current_row->file, 'public://aw_files');
    } 

  }

}





class AWDocMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');
    
    $query->leftJoin('documents', 'aw_d', 'aw_o.oid = aw_d.docid');
    
    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status'))
      ->fields('aw_d', array('content'))
      ->condition('aw_o.class_id', 7)
      ->orderBy('aw_o.modified', 'DESC');
        
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           

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

/*    
    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_reference_image', 'reference_image')
      ->separator('|');
    }

    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_reference_file', 'reference_image')
      ->separator('|');
    }
*/

  }

  public function prepareRow($current_row) {

     $current_row->uid = _aw_get_uid($current_row->createdby);
     //   $current_row->body =  $current_row->content;

  $current_row->body =  _aw_migrate_clean_text($current_row->content, $current_row->oid) . '\n---\n' . $current_row->content;
  
  
     /*
     $aliases = array();$a = array();
 
     $connection = Database::getConnection('default', 'aw');

     $query = $connection->select('aliases', 'aw_a');

     $query->fields('aw_a', array('source', 'target', 'type', 'idx'))
          ->condition('source', $current_row->oid)
          ->execute();

        $q = $query->countQuery()->execute()->fetchField();
    print_r($query);   
     $current_row->body = implode('|', $a) . $current_row->body;
     */
   
   }
}

function _aw_get_uid($user_name) {
  
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

function _aw_alias_link($matches) {
  return '[[['.$matches[1].']]]';
}

function _aw_migrate_clean_text($text, $oid = NULL) {   

  $text = preg_replace_callback('#\#file([0-9]+)(.*)\##', '_aw_alias_link', $text);
 
  // then run another pass over the html (twice), removing unwanted attributes

  $text = str_replace(array('<BR>', '<br>', '</P>', '</p>', '</PRE>', '</pre>'), "\n", $text);
    
    $preg_replace_map = array(
      '#<span style=\"font-weight: bold;\">(.*)</span>#e' => "('<strong>$1</strong>')",
      '#<!--(.*)-->#e' => '',
      '#<?(.*)>#e' => '',
      '#<strong>([\s]+)</strong>#e' => '',

//     '#\#file([0-9]+)(.*)\##e' => "('[[file $1]]')",
//      '#\#pict([0-9]+)(.*)\##e' => "('[[pict $1]]')",
//      '#\#link([0-9]+)(.*)\##e' => "('[[link $1]]')",
    );
    
    $text = preg_replace(array_keys($preg_replace_map), array_values($preg_replace_map), $text);

    // Allowing all HTML tags except html, body, div, span, fieldset, form, input, select, textarea, label, legend, font
    // pre is now allowed because of MSWord dumps
   
    $allowed_tags = array(
      'applet','object','iframe','h1','h2','h3','h4','h5','h6',
      'p','blockquote','a','abbr','acronym','address','big','cite','code',
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
      '<P>' => '',
      '<p>' => '',
      '<STRONG>' => '<strong>',
      '</STRONG>' => '</strong>',
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
      '&#8222;' => '"',
      '&#8220;' => '"',
      '&acute;' => '´',
      '&Scaron;' => 'Š',
      '&scaron;' => 'Š',
      'ð' => 'ž',
      '&Otilde;' => 'Õ',
      '&otilde;' => 'õ',
      '&Auml;' => 'Ä',
      '&auml;' => 'ä',
      '&Ouml;' => 'Ö',
      '&ouml;' => 'Ö',
      '&Uuml;' => 'Ü',
      '&uuml;' => 'ü',
      '&#8722;' => '−',
      '<A ' => '<a ',
      '</A>' => '</a>',
    );
  
    $text = str_replace(array_keys($replace_map), array_values($replace_map), $text);

    return $text;

}



function _aw_copy_file($file_path, $target_dir) {
  if (variable_get('aw_file_path', '')) {
    $file = basename($file_path);
    $web_file_fullpath = variable_get('aw_file_path', '') . $file;
    print_r($web_file_fullpath).'|';
    $web_file = file_get_contents($web_file_fullpath);
    file_unmanaged_save_data($web_file, $target_dir . '/' . $file, 'FILE_EXISTS_REPLACE');
  }
}