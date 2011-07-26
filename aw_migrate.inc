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



class AWDocMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');
    
    $query->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status', 'metadata'))
          ->condition('class_id', 7)
          ->orderBy('modified', 'DESC');
        
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
    $this->addFieldMapping('path', 'path');
    
    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_reference_image', 'reference_image')
      ->separator('|');
    }

    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_reference_file', 'reference_image')
      ->separator('|');
    }

  }

  public function prepareRow($current_row) {
    
     $current_row->path = 'doc/' . $current_row->oid;
     
     $query = db_select('users', 'u')
        ->fields('u', array('uid'))
        ->condition('name', $current_row->createdby)
        ->execute();
      
     foreach ($query as $user) {
       $current_row->uid = $user->uid;
     }
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
  
    $query = $connection->select('documents', 'a_d');
  
    $query->fields('a_d', array('content'))
          ->condition('docid', $current_row->oid)
          ->execute();

    foreach ($query as $doc) {
      $current_row->body =  _aw_migrate_cleanup($doc->content);
    }
    
  }

}



function _aw_migrate_cleanup($text) {   
    
    $preg_replace_map = array(
      '#\#file([0-9]+)\##e' => '',
      '#\#pict([0-9]+)\##e' => '',
      '#\#minigalerii([0-9]+)\##e' => '',
      '#\s\s+#' => ' ',
      '#<span style=\"font-weight: bold;\">(.*)</span>#e' => "('<strong>$1</strong>')",
      '#<!--(.*)-->#e' => '',
      '#<strong>([\s]+)</strong>#e' => '',
      '#<strong>GALLERY</strong>#e' => '',
    );
    
    $current_row->content_filtered = preg_replace(array_keys($preg_replace_map), array_values($preg_replace_map), $text);


    // Omitting html, body, div, span, fieldset, form, input, select, textarea, label, legend, font

      $allowed_tags = array(
      'applet','object','iframe','h1','h2','h3','h4','h5','h6',
      'p','blockquote','pre','a','abbr','acronym','address','big','cite','code',
      'del','dfn','em','img','ins','kbd','q','s','samp','small','strike',
      'strong','sub','sup','tt','var','b','u','i','center','dl','dt','dd','ol',
      'ul','li', 'table','caption','tbody','tfoot','thead','tr','th','td'
      );

      $text = filter_xss($text, $allowed_tags);

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
  
    $text = str_replace(array_keys($replace_map), array_values($replace_map), $text);

return $text;

}