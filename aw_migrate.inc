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
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');

    $query->leftJoin('images', 'aw_i', 'aw_o.oid = aw_i.id');

    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status'))
      ->fields('aw_i', array('file'))
      ->condition('aw_o.class_id', 6)
      ->orderBy('aw_o.modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           
    $this->destination = new MigrateDestinationFile(array('copy_file' => TRUE));
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
          )
        ),
        MigrateDestinationFile::getKeySchema()
      );
  
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('timestamp', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('uri', 'uri');

    }

  public function prepareRow($current_row) {

    $current_row->uid = _aw_get_uid($current_row->createdby);
    $current_row->uri = variable_get('aw_domain', '') . '/orb.aw/class=image/action=show/fastcall=1/file=' . basename($current_row->file);
    // $current_row->uri = 'http://localhost/sites/aw_test/sites/default/files/aw_files/' . basename($current_row->file);
    print_r($current_row->uri);
  }

}



class AWFileMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');

    $query->leftJoin('files', 'aw_f', 'aw_o.oid = aw_f.id');

    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status'))
      ->fields('aw_f', array('file'))
      ->condition('aw_o.class_id', 41)
      ->orderBy('aw_o.modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           
    $this->destination = new MigrateDestinationFile(array('copy_file' => TRUE));
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
          )
        ),
        MigrateDestinationFile::getKeySchema()
      );
  
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('timestamp', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('uri', 'uri');

    }

  public function prepareRow($current_row) {

    $current_row->uid = _aw_get_uid($current_row->createdby);
    $current_row->uri = variable_get('aw_domain', '') . '/orb.aw/class=file/action=preview/id='. $current_row->oid . '/' . basename($current_row->file);
    // $current_row->uri = 'http://localhost/sites/aw_test/sites/default/files/aw_files/' . basename($current_row->file);

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
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status', 'alias'))
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
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('path', 'alias');
    $this->addFieldMapping('body', 'body')->arguments(array('format' => 'full_html'));

  }

  public function prepareRow($current_row) {

    $current_row->uid = _aw_get_uid($current_row->createdby);

    $current_row->title = _aw_migrate_clean_text($current_row->name);
    $current_row->body =  _aw_migrate_clean_text($current_row->content, $current_row->oid) 
   
    // Enable for debugging
    // $current_row->body =  _aw_migrate_clean_text($current_row->content, $current_row->oid). '<br />-------<br />' . $current_row->content;  
  
   }
}


// @TODO move these utility functions below to separate inc file

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


class aliasProcessor {
  public $oid = NULL;
  public function process($matches) {
 
 // Done: link, file, pict
 // @TODO layout, document (7), sitemap, s
 // minigalerii, list, webform, bannerplace, forum, poll, event_webview
 
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

          $output = l($name ? $name : $url, $url); // @TODO if no name in extlinks table, try objects
          break;      

  
      default:
        $output = $matches[0]; // Pass through
        break;
    }
    
    return $output;
  }
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
