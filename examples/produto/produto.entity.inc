<?php
/**
 * @file
 * Classes das entidades do módulo Produto.
 */

/**
 * Controller de UI da entidade Produto Type.
 */
class ProdutoTypeUIController extends EntityDefaultUIController {

  /**
   * Complementa o hook_menu() padrão.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Gerenciar tipos de produtos, inclusive fields.';
    return $items;
  }

}

/**
 * Controller de classe da entidade Produto Type.
 */
class ProdutoTypeController extends EntityAPIControllerExportable {

}

/**
 * Controller de classe da entidade Produto.
 */
class ProdutoController extends EntityAPIController {

  /**
   * Adiciona as propriedades da entidade ao display.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    // Autor.
    $user = user_load($entity->uid);
    $content['uid'] = array(
      '#theme' => 'field',
      '#title' => t('Autor'),
      '#access' => TRUE,
      '#label_display' => 'inline',
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'produto',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->uid)),
      '#formatter' => 'text_default',
      0 => array('#markup' => l($user->name, 'user/' . $entity->uid)),
    );

    // SKU.
    $content['sku'] = array(
      '#theme' => 'field',
      '#title' => t('SKU'),
      '#access' => TRUE,
      '#label_display' => 'inline',
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'produto',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->sku)),
      '#formatter' => 'text_default',
      0 => array('#markup' => $entity->sku),
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }

}

/**
 * Classe para a entidade Produto Type.
 */
class ProdutoType extends Entity {

  public $name;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'produto_type');
  }

  /**
   * Retorna se esta entidade está "trancada" e, portanto, não consegue ser
   * excluída ou renomeada. Tipos de entidade que são fornecidas no código são
   * tratadas como trancadas.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

}

/**
 * Classe para a entidade Produto.
 */
class Produto extends Entity {

  /**
   * O id do produto.
   *
   * @var integer
   */
  public $pid;

  /**
   * O nome do produto.
   *
   * @var string
   */
  public $title;

  /**
   * O tipo do produto.
   *
   * @var string
   */
  public $type;

  /**
   * O id do autor.
   *
   * @var integer
   */
  public $uid;

  public function __construct($values = array()) {
    if (isset($values['type'])) {
      if ($values['type'] instanceof ProdutoType) {
        $values['type'] = $values['type']->name;
      }
      else {
        $values['type'] = $values['type'];
      }
    }

    if (!isset($values['uid'])) {
      $user = user_uid_optional_load();
      $values['uid'] = $user->uid;
    }

    parent::__construct($values, 'produto');
  }

  /**
   * Returns the full url() for the produto.
   */
  public function url() {
    $uri = $this->uri();
    return url($uri['path'], $uri);
  }

  /**
   * Returns the drupal path to this produto.
   */
  public function path() {
    $uri = $this->uri();
    return $uri['path'];
  }

  protected function defaultUri() {
    return array(
      'path' => 'produto/' . $this->pid,
    );
  }

}

/**
 * Controller de metadados para a entidade Produto.
 */
class ProdutoMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    // Complementa a propriedade "type".
    $properties['type'] = array(
      'type' => 'produto_type',
      'setter permission' => 'administer produto',
      'required' => TRUE,
      'description' => t('O tipo de Produto.'),
    ) + $properties['type'];

    // Substitui a propriedade "uid" com um relacionamento à entidade do usuário.
    $properties['uid'] = array(
      'label' => t('User'),
      'type' => 'user',
      'description' => t('O autor do produto.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer produto',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    return $info;
  }
}